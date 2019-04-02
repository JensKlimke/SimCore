clc;

figure(2);
clf;

v = 10:10:20
v0 = 10:10:30

dv = v0 - v';

a = sign(dv) .* (1 - (1 - abs(dv) ./ v0) .^ 4)


% plot(v, a);

ds = 120:-1:0;
dsMax = 100;

f = min(1.0, max(0.0, (1 - (ds / dsMax) .^ 1.5)))

clf;
plot(dsMax - ds, f)


