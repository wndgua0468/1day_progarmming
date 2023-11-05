clc;

time = 25; % 총 시간
t = [0:0.01:0.01*time];
m = 80.0;
b = 50.0;
g = 9.81;

v = zeros(time+1, 1);
k1 = zeros(time+1, 1);
k2 = zeros(time+1, 1);
k3 = zeros(time+1, 1);
k4 = zeros(time+1, 1);

v(1) = 20;

for i = 1:1:time
    k1(i) = (0.01) * (g - b/m * v(i)^2);
    k2(i) = (0.01) * (g - b/m * (v(i) + k1(i)/2)^2);
    k3(i) = (0.01) * (g - b/m * (v(i) + k2(i)/2)^2);
    k4(i) = (0.01) * (g - b/m * (v(i) + k3(i))^2);
    
    if (i <= time)
        v(i+1) = v(i) + ((1/6) * (k1(i) + 2*k2(i) + 2*k3(i) + k4(i)));
    end
end

fprintf("t         v        k1      k2      k3      k4 \n");
for i = 1:time
    fprintf("%3.4f  %3.4f %3.4f %3.4f %3.4f %3.4f \n", t(i), v(i), k1(i), k2(i), k3(i), k4(i));
end

plot(t, v);
title('Runge Kutta v(m/s)');
xlabel('t');
ylabel('V', 'Rotation', 0);
