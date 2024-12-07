tab = readtable('../input/AB_NYC_2019.csv', 'ReadVariableNames', false);
ds_mean = mean(tab.Var10);
ds_var = var(tab.Var10);
fileIdMean = fopen("output_mean_matlab.txt", 'wb');
fileIdVar = fopen("output_var_matlab.txt", 'wb');
fprintf(fileIdMean,  "%.4f", ds_mean);
fprintf(fileIdVar,  "%.4f", ds_var);
fclose(fileIdMean);
fclose(fileIdVar);