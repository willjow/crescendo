#!/usr/bin/env python

def main(args):
    """Very simple script to guess at driver ADC values for useful voltages,
    based on readings taken at other voltages.

    Input lines should be of the format:
        {ADC value} - {Voltage}V
    """
    readings_path = args[0]
    fit_name = args[1]

    fit = FIT_FUNCTIONS[fit_name]
    measurements = {}
    table = {}
    with open(readings_path) as readings:
        for line in readings:
            if line.startswith('#'):
                continue
            line = line.strip()
            print(f"Line: {line}")
            parts = line.split()
            if line.endswith('V'):
                adc = int(parts[0])
                volts = float(parts[2][:-1])
                measurements[volts] = adc
        table = fit(measurements)

    for volts, adc in table.items():
        print(f"#define ADC_{volts}     {adc}")


def _fit_linear(measurements):
    """Fits linearly based on the min/max measurements."""
    v_lowest = min(measurements.keys())
    v_highest = max(measurements.keys())
    adc_lowest = measurements[v_lowest]
    adc_highest = measurements[v_highest]
    v_range = v_highest - v_lowest
    adc_range = adc_highest - adc_lowest
    adc_per_v = adc_range / v_range

    table = {}
    for target_v in range(44, 19, -1):
        delta_volts = (target_v / 10.0) - v_lowest
        adc = adc_lowest + (adc_per_v * delta_volts)
        table[target_v] = round(adc)

    return table


def _fit_reciprocal(measurements):
    """Fits according to (1.1 / v * 1024 + offset), for some fixed offset which
    minimizes the mean squared error w.r.t. our observations.
    """
    model = lambda v: 1.1 / v * 1024
    n = len(measurements)
    mean_offset = sum(adc - model(v) for v, adc in measurements.items()) / n

    table = {}
    for target_v in range(44, 19, -1):
        volts = (target_v / 10.0)
        table[target_v] = round(model(volts) + mean_offset)

    return table


FIT_FUNCTIONS = {"linear": _fit_linear,
                 "reciprocal": _fit_reciprocal,
                }


if __name__ == "__main__":
    import sys
    main(sys.argv[1:])

