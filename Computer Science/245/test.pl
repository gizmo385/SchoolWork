power(L,0,1).

power(L,M,N) :- D is M-1, power(L,D,P), N is P*L.
