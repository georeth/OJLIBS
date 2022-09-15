Binomial numbers -> polynomial coefficients -> convolution

A
sum{a in A} f(a)

A=\union{B_i} disjoint
sum{a in A} f(a) = sum{i in I} sum{a in B_i} f(a)

hint:
1. any a in B_i: f(a) = g(B_i)
2. use structure of A. e.g. g(g(a))=a, f(g(a)) + f(a) = 0

sum{a in A} f(a) = sum{k} #{ f(a) = k } * k

hint: use inversion tranformation
1. transform{k} #{ f(a) <= k } = #{ f(a) = k }
2. transform{k} #{ f(a) is divisor of k } = #{ f(a) = k }
3. transform{k} #{ f(a) is multiple of k } = #{ f(a) = k }
4. The principle of inclusion and exclusion


polynomial library:
poly_eval(): Horner's
poly_shift() or poly_translate(): https://cs.stackexchange.com/questions/77842/efficient-algorithm-to-translate-shift-polynomials
poly_mul()
poly_div()
