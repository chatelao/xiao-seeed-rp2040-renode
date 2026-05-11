import re
import subprocess
import os
import sys

def parse_motor_models(filepath):
    models = []
    with open(filepath, 'r') as f:
        content = f.read()

    # Find the summary table
    table_match = re.search(r'\| Model ID \|.*?\| Typical Behavior \|\n\| :--- \|.*?\|\n((?:\|.*?\n)+)', content, re.DOTALL)
    if not table_match:
        raise ValueError("Could not find motor models table in " + filepath)

    table_rows = table_match.group(1).strip().split('\n')
    for row in table_rows:
        cols = [c.strip() for c in row.split('|') if c.strip()]
        if len(cols) < 8:
            continue

        model = {
            'id': cols[0].replace('**', ''),
            'Kv': float(cols[2]),
            'R': float(cols[3]),
            'J': float(cols[4]),
            'B': float(cols[5]),
            'Vstart': float(cols[6]),
            'Vstop': float(cols[7])
        }
        models.append(model)
    return models

def run_spice_simulation(model):
    netlist = f"""
* Motor Verification: {model['id']}
.params Kv={model['Kv']} R_arm={model['R']} J_c={model['J']} B_r={model['B']} Vstart={model['Vstart']} Vstop={model['Vstop']}
.params Ke={{60/(2*3.14159265*Kv)}}

* Triangular Input Voltage: 0V -> 12V in 10s, 12V -> 0V in 10s
Vin IN 0 PWL(0 0 10 12 20 0)

* Behavioral Input Voltage with Stiction
* V_eff = (velocity <= 0.01) ? ( (V_in >= Vstart) ? V_in : 0 ) : ( (V_in >= Vstop) ? V_in : 0 )
Beff EFF 0 V = (V(OMEGA) <= 0.01) ? ( (V(IN) >= Vstart) ? V(IN) : 0 ) : ( (V(IN) >= Vstop) ? V(IN) : 0 )

* --- Electrical Side ---
R1 EFF 3 {{R_arm}}
* Back-EMF: V = Ke * omega
Bbemf 3 0 V=Ke*V(OMEGA)

* --- Mechanical Side ---
* Torque: T = Ke * I(R1)
Btorque 0 OMEGA I=Ke*I(R1)
* Inertia: T = J * d(omega)/dt
C1 OMEGA 0 {{J_c}}
* Viscous Friction: T = B * omega
Rfric OMEGA 0 {{1/B_r}}

.control
tran 1m 20
wrdata output.txt V(IN) V(OMEGA) V(EFF)
exit
.endcontrol
.end
"""
    with open("sim.net", "w") as f:
        f.write(netlist)

    result = subprocess.run(["ngspice", "-b", "sim.net"], capture_output=True, text=True)
    if result.returncode != 0:
        print(f"Error running ngspice: {result.stderr}")
        return None, None

    # Analyze output.txt
    # Format: time val1 val2 ...
    v_start_sim = None
    v_stop_sim = None

    with open("output.txt", "r") as f:
        lines = f.readlines()

    for line in lines:
        parts = line.split()
        if not parts: continue
        t = float(parts[0])
        vin = float(parts[1])
        omega = float(parts[3])
        veff = float(parts[5])

        # Going up (0 to 10s)
        if t <= 10.0 and v_start_sim is None:
            # Detect when EFF becomes non-zero
            if veff > 0.1:
                v_start_sim = vin

        # Going down (10s to 20s)
        if t > 10.0 and v_stop_sim is None:
            # Detect when EFF becomes zero while it was previously non-zero
            if veff < 0.1:
                v_stop_sim = vin

    return v_start_sim, v_stop_sim

def main():
    models = parse_motor_models("LOCO_MOTOR_MODELS.md")
    failed = False

    print(f"{'Model ID':<12} | {'Vstart':<8} | {'Sim Start':<10} | {'Vstop':<8} | {'Sim Stop':<10} | Status")
    print("-" * 75)

    for m in models:
        v_start_sim, v_stop_sim = run_spice_simulation(m)

        # Check tolerances (0.2V due to ramp speed and threshold)
        start_ok = abs(v_start_sim - m['Vstart']) < 0.2 if v_start_sim is not None else False
        stop_ok = abs(v_stop_sim - m['Vstop']) < 0.2 if v_stop_sim is not None else False

        status = "OK" if start_ok and stop_ok else "FAIL"
        if status == "FAIL":
            failed = True

        print(f"{m['id']:<12} | {m['Vstart']:<8.2f} | {v_start_sim if v_start_sim is not None else 0.0:<10.2f} | {m['Vstop']:<8.2f} | {v_stop_sim if v_stop_sim is not None else 0.0:<10.2f} | {status}")

    # Cleanup
    if os.path.exists("sim.net"): os.remove("sim.net")
    if os.path.exists("output.txt"): os.remove("output.txt")

    if failed:
        print("\nVerification FAILED")
        sys.exit(1)
    else:
        print("\nVerification SUCCESS")
        sys.exit(0)

if __name__ == "__main__":
    main()
