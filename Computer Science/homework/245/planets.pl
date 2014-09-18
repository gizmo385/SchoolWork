%planets.pl			Christopher Chapline 			SL: Nick Topping

%Fact declarations
orbits(mercury, sun).
orbits(venus, sun).
orbits(earth, sun).
orbits(mars, sun).
orbits(deimos,mars).
orbits(phobos,mars).
orbits(moon, earth).


%Rules

%Something is a planet if it directly orbits the Sun
planet(P) :- orbits(P, sun).

%Something is a moon if it orbits something that orbits the Sun
moon(M) :- orbits(M, X) , orbits(X, sun).

%Something is moonless if it orbits the Sun and nothing orbits it
moonless(P) :- orbits(P, sun) , \+orbits(X, P).

%Two heavenly bodies are siblings if they are different objects that orbit the same third body
siblings(X, Y) :- X \= Y , orbits(X,Z) , orbits(Y,Z).

%Some object X super-obits Y if X orbit some Z that orbits Y
superorbits(X, Y) :- orbits(X,Z) , orbits(Z, Y).