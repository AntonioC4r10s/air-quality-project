import numpy as np

INTERATIONS = 5


values = [350,385,350,421,325,321,325]

def calibration_MQ9():
    x1 = np.log10(200)
    x2 = np.log10(1000)
    y1 = np.log10(1.6)
    y2 = np.log10(0.8)

    CO_CURVE = [x1, y1, (y2-y1)/(x2-x1)]
    print(f'CO_curve: {CO_CURVE}')

    value = sum(values)/len(values)
    sensor_voltage = float((value/1024)*5.0)

    RS = (5.0-sensor_voltage)/sensor_voltage
    R0 = RS/9.9
    ratio = RS/R0

    #return RS, R0, ratio

    print(f'RS: {calibration_MQ9()[0]}\nR0: {calibration_MQ9()[1]}\nRatio: {calibration_MQ9()[2]}')
    ppm = np.power(10, ((np.log10(rs_ro) - CO_CURVE[1]) / CO_CURVE[2]) + CO_CURVE[0])
    print(f'PPM: {ppm}')

calibration_MQ9()