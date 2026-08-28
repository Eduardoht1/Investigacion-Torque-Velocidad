clc
close all

filename = 'motor_vel.csv';

datos = readtable(filename);

tiempo = datos.Segundos;
rpm = datos.RPM;

%Figura
figure(); 
plot(tiempo, rpm, 'b-', 'LineWidth', 1.5);

title('Respuesta de Velocidad del Motor en el Tiempo', 'FontSize', 12);
xlabel('Tiempo (s)', 'FontSize', 11);
ylabel('Velocidad (RPM)', 'FontSize', 11);
grid on;

% Ajustar los límites del eje X para que coincidan exactamente con la duración (0 a 5 seg)
xlim([0.6 max(tiempo)]);