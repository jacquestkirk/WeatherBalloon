%load 'TS91ResTemp'
%load 'B57164K0102'
load 'NTCLE203E3302SB0'
temps_degC = data(:,1);
resistances_ohms = data(:,2);



%%
ref_res_ohms = 51000;


pct_full_scale = ref_res_ohms./(resistances_ohms + ref_res_ohms) * 100;


temp_res = temps_degC(2)-temps_degC(1);
temp_centered_degC = temps_degC + temp_res;
temp_centered_degC = temp_centered_degC(1:end-1);

pct_full_scale_per_degC = diff(pct_full_scale)/temp_res;

figure(1)
plot(temps_degC, pct_full_scale)
ylabel('Percentage of full scale (%)')
xlabel('Temp (degC)')
figure(2)
plot(temp_centered_degC, pct_full_scale_per_degC)
xlim([-55,40])
ylabel('Percentage change per deg C (%)')
xlabel('Temp (degC)')

figure(3)
plot(temps_degC, resistances_ohms)
ylabel('Resistance (Ohms)')
xlabel('Temp (degC)')



