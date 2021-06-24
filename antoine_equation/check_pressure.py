import CoolProp

pressure_vector = [200.0, 500.0, 800.0, 1100.0, 1400.0, 1700.0, 2000.0, 2300.0, 2700.0, 3200.0]

for press in pressure_vector:
    print(CoolProp.CoolProp.PropsSI('T','P', press * 1e3,'Q',1,'R32')-273.15)