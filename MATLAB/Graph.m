samp_rate = 1000; 
range = 100; 
begin = 1 
offset = 1861; 
data = load('DATA0_NEW.txt'); 
x = 0:1/samp_rate:(range-1)/samp_rate;
 
plot(x(begin:range),(data(begin:range,1)/4095*3.3)); %-offset)/max(data(:,1)-offset));
hold on 
plot(x(begin:range),(data(begin:range,2))/4095*3.3);
plot(x(begin:range),(data(begin:range,3))/4095*3.3)%-offset)/max(data(:,3)-offset));
plot(x(begin:range),(data(begin:range,4))/4095*3.3);%-offset)/max(data(:,4)-offset));
plot(x(begin:range),(data(begin:range,5))/4095*3.3);%-offset)/max(data(:,5)-offset));
plot(x(begin:range),(data(begin:range,6))/4095*3.3);%-offset)/max(data(:,6)-offset))
hold off

axis([x(begin) x(range) -.5 3.3])
xlabel('Time(s)','FontSize',14); 
ylabel('Voltage Level(V)','FontSize',14); 
grid on
