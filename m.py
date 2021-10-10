from math import exp
from math import log
from math import e
from math import sqrt
# x = (exp(1 / e) - exp(-e)) / 2
x = exp(-e) + 0.0001
print("num1 = ", exp(1 / e));
print("num2 = ", exp(-e));
print("x = ", x)
print("-lnx = ", -log(x));
res = x
for i in range(1000):
	for j in range(i):
		res = x ** res
	print(res)