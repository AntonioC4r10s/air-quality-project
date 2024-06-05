import numpy as np
import pandas as pd

# Calibrar os sensor para obter r0 para a coversão de CO em rs/r0 para PPM
def calibration_MQ9_CO(values):

    value = np.mean(values)
    sensor_voltage = (value/1024)*5.0
    rs_ar = (5.0-sensor_voltage)/sensor_voltage
    r0 = rs_ar/10
    ratio = rs_ar/r0

    print(f'\nvalue: {value}, '+
          f'sensor_voltage: {sensor_voltage}, '+
          f'rs_ar: {rs_ar}, '+
          f'r0: {r0}, '+
          f'ratio: {ratio}')
    return r0


# Retornar valor coletado da tensão para PPM
def CO_in_PPM(value, r0):
    
    sensor_voltage = (value/1024)*5.0
    rs_co = (5.0-sensor_voltage)/sensor_voltage
    ratio = rs_co/r0
    print(f'\nvalue: {value}, '+
        f'sensor_voltage: {sensor_voltage}, '+
        f'rs_co: {rs_co}, '+
        f'r0: {r0}, '+
        f'ratio: {ratio}')
    
    co_p1 = [np.log10(200), np.log10(2.8)]
    co_p2 = [np.log10(1000), np.log10(0.8)]
    inclinat = (co_p2[1]-co_p1[1])/(co_p2[0]-co_p1[0])

    print(f'\nco_p1: {co_p1}, ' +
        f'co_p2: {co_p2}, ' +
        f'inclinat: {inclinat}\n')

    ppm = np.power(10, ((np.log(ratio) - co_p1[1]) / inclinat) + co_p1[0])
    # print(f'ppm: {ppm}\n')
    return ppm