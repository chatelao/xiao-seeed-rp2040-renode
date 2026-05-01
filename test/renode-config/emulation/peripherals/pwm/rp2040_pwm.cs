/**
 * rp2040_pwm.cs
 *
 * Copyright (c) 2024 Jules
 *
 * Distributed under the terms of the MIT License.
 */

using System;
using System.Collections.Generic;
using Antmicro.Renode.Core;
using Antmicro.Renode.Core.Structure.Registers;
using Antmicro.Renode.Logging;
using Antmicro.Renode.Peripherals.Bus;
using Antmicro.Renode.Utilities;

namespace Antmicro.Renode.Peripherals.PWM
{
    public class RP2040PWM : RP2040PeripheralBase, INumberedGPIOOutput
    {
        public RP2040PWM(IMachine machine, ulong address) : base(machine, address)
        {
            IRQ = new GPIO();
            var innerConnections = new Dictionary<int, IGPIO>();
            PWMOut = new GPIO[NumberOfSlices * 2];
            for (int i = 0; i < PWMOut.Length; i++)
            {
                PWMOut[i] = new GPIO();
                innerConnections[i] = PWMOut[i];
            }
            Connections = innerConnections;

            slices = new PWMSlice[NumberOfSlices];
            for (int i = 0; i < NumberOfSlices; ++i)
            {
                slices[i] = new PWMSlice(this, i);
            }
            DefineRegisters();
            Reset();
        }

        private void DefineRegisters()
        {
            Registers.CH0_CSR.DefineMany(this, NumberOfSlices, (reg, index) =>
            {
                reg.WithFlag(0, out slices[index].enabled, name: $"CH{index}_CSR_EN", writeCallback: (_, __) => slices[index].Update())
                   .WithFlag(1, out slices[index].phaseCorrect, name: $"CH{index}_CSR_PH_CORRECT", writeCallback: (_, __) => slices[index].Update())
                   .WithValueField(2, 2, out slices[index].divMode, name: $"CH{index}_CSR_DIVMODE")
                   .WithFlag(4, out slices[index].invertA, name: $"CH{index}_CSR_A_INV", writeCallback: (_, __) => slices[index].Update())
                   .WithFlag(5, out slices[index].invertB, name: $"CH{index}_CSR_B_INV", writeCallback: (_, __) => slices[index].Update())
                   .WithFlag(6, name: $"CH{index}_CSR_TOP_SEL")
                   .WithFlag(7, name: $"CH{index}_CSR_ADVANCE")
                   .WithReservedBits(8, 24);
            }, stepInBytes: 0x14);

            Registers.CH0_DIV.DefineMany(this, NumberOfSlices, (reg, index) =>
            {
                reg.WithValueField(0, 4, out slices[index].divFrac, name: $"CH{index}_DIV_FRAC", writeCallback: (_, __) => slices[index].Update())
                   .WithValueField(4, 8, out slices[index].divInt, name: $"CH{index}_DIV_INT", writeCallback: (_, __) => slices[index].Update())
                   .WithReservedBits(12, 20);
            }, stepInBytes: 0x14);

            Registers.CH0_CTR.DefineMany(this, NumberOfSlices, (reg, index) =>
            {
                reg.WithValueField(0, 16, out slices[index].counter, name: $"CH{index}_CTR")
                   .WithReservedBits(16, 16);
            }, stepInBytes: 0x14);

            Registers.CH0_CC.DefineMany(this, NumberOfSlices, (reg, index) =>
            {
                reg.WithValueField(0, 16, out slices[index].compareA, name: $"CH{index}_CC_A", writeCallback: (_, __) => slices[index].Update())
                   .WithValueField(16, 16, out slices[index].compareB, name: $"CH{index}_CC_B", writeCallback: (_, __) => slices[index].Update());
            }, stepInBytes: 0x14);

            Registers.CH0_TOP.DefineMany(this, NumberOfSlices, (reg, index) =>
            {
                reg.WithValueField(0, 16, out slices[index].top, name: $"CH{index}_TOP", writeCallback: (_, __) => slices[index].Update())
                   .WithReservedBits(16, 16);
            }, stepInBytes: 0x14);

            Registers.EN.Define(this)
                .WithValueField(0, 8, writeCallback: (_, val) =>
                {
                    for (int i = 0; i < 8; i++)
                    {
                        slices[i].enabled.Value = BitHelper.IsBitSet(val, (byte)i);
                        slices[i].Update();
                    }
                }, valueProviderCallback: _ =>
                {
                    uint val = 0;
                    for (int i = 0; i < 8; i++)
                    {
                        if (slices[i].enabled.Value) val |= (1u << i);
                    }
                    return val;
                }, name: "EN");

            Registers.INTR.Define(this)
                .WithValueField(0, 8, name: "INTR");
            Registers.INTE.Define(this)
                .WithValueField(0, 8, name: "INTE");
            Registers.INTF.Define(this)
                .WithValueField(0, 8, name: "INTF");
            Registers.INTS.Define(this)
                .WithValueField(0, 8, name: "INTS");
        }

        public override void Reset()
        {
            base.Reset();
            foreach (var slice in slices)
            {
                slice.Update();
            }
        }

        public double GetDutyCycleA(int slice) => slices[slice].DutyCycleA;
        public double GetDutyCycleB(int slice) => slices[slice].DutyCycleB;
        public double GetFrequency(int slice) => slices[slice].Frequency;

        public GPIO IRQ { get; private set; }
        public IReadOnlyDictionary<int, IGPIO> Connections { get; }
        private GPIO[] PWMOut { get; }

        private readonly PWMSlice[] slices;
        private const int NumberOfSlices = 8;

        private class PWMSlice
        {
            public PWMSlice(RP2040PWM parent, int index)
            {
                this.parent = parent;
                this.index = index;
            }

            public void Update()
            {
                if (!enabled.Value)
                {
                    parent.PWMOut[2 * index].Set(invertA.Value);
                    parent.PWMOut[2 * index + 1].Set(invertB.Value);
                    return;
                }

                double topVal = top.Value + 1;
                double dcA = (double)compareA.Value / topVal;
                double dcB = (double)compareB.Value / topVal;

                if (dcA > 1.0) dcA = 1.0;
                if (dcB > 1.0) dcB = 1.0;

                if (invertA.Value) dcA = 1.0 - dcA;
                if (invertB.Value) dcB = 1.0 - dcB;

                double divisor = divInt.Value + (divFrac.Value / 16.0);
                if (divisor == 0) divisor = 1.0;

                double period = phaseCorrect.Value ? (2.0 * top.Value) : (top.Value + 1.0);
                if (period == 0) period = 1.0;

                double freq = 125000000.0 / divisor / period;

                parent.Log(LogLevel.Info, "PWM Slice {0}: Frequency {1}Hz, Duty A {2:P}, Duty B {3:P}", index, freq, dcA, dcB);

                // Update GPIO state based on Duty Cycle > 0 for basic visualization
                parent.PWMOut[2 * index].Set(dcA > 0);
                parent.PWMOut[2 * index + 1].Set(dcB > 0);
            }

            public double DutyCycleA => (double)compareA.Value / (top.Value + 1);
            public double DutyCycleB => (double)compareB.Value / (top.Value + 1);
            public double Frequency
            {
                get
                {
                    double divisor = divInt.Value + (divFrac.Value / 16.0);
                    if (divisor == 0) divisor = 1.0;
                    double freq = 125000000.0 / divisor / (top.Value + 1);
                    if (phaseCorrect.Value) freq /= 2.0;
                    return freq;
                }
            }

            public IFlagRegisterField enabled;
            public IValueRegisterField divMode;
            public IFlagRegisterField phaseCorrect;
            public IFlagRegisterField invertA;
            public IFlagRegisterField invertB;
            public IValueRegisterField divFrac;
            public IValueRegisterField divInt;
            public IValueRegisterField counter;
            public IValueRegisterField compareA;
            public IValueRegisterField compareB;
            public IValueRegisterField top;

            private readonly RP2040PWM parent;
            private readonly int index;
        }

        private enum Registers
        {
            CH0_CSR = 0x00,
            CH0_DIV = 0x04,
            CH0_CTR = 0x08,
            CH0_CC = 0x0c,
            CH0_TOP = 0x10,
            // Slices 1-7 follow at 0x14 intervals
            EN = 0xa0,
            INTR = 0xa4,
            INTE = 0xa8,
            INTF = 0xac,
            INTS = 0xb0
        }
    }
}
