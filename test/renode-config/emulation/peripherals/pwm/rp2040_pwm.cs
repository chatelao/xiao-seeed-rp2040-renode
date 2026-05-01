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
                   .WithValueField(1, 2, out slices[index].divMode, name: $"CH{index}_CSR_DIVMODE")
                   .WithFlag(3, out slices[index].phaseCorrect, name: $"CH{index}_CSR_PH_CORRECT")
                   .WithFlag(4, out slices[index].invertA, name: $"CH{index}_CSR_A_INV", writeCallback: (_, __) => slices[index].Update())
                   .WithFlag(5, out slices[index].invertB, name: $"CH{index}_CSR_B_INV", writeCallback: (_, __) => slices[index].Update())
                   .WithValueField(6, 1, name: $"CH{index}_CSR_TOP_SEL")
                   .WithFlag(7, name: $"CH{index}_CSR_ADVANCE")
                   .WithReservedBits(8, 24);
            }, stepInBytes: 0x14);

            Registers.CH0_DIV.DefineMany(this, NumberOfSlices, (reg, index) =>
            {
                reg.WithValueField(0, 8, out slices[index].divFrac, name: $"CH{index}_DIV_FRAC")
                   .WithValueField(8, 8, out slices[index].divInt, name: $"CH{index}_DIV_INT")
                   .WithReservedBits(16, 16);
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
                bool enabledValue = enabled.Value;
                bool stateA = enabledValue && (compareA.Value > 0);
                bool stateB = enabledValue && (compareB.Value > 0);

                if (invertA.Value) stateA = !stateA;
                if (invertB.Value) stateB = !stateB;

                parent.PWMOut[2 * index].Set(stateA);
                parent.PWMOut[2 * index + 1].Set(stateB);
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
