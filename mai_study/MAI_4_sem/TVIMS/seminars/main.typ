#import "@preview/grape-suite:3.1.0": exercise
#import exercise: project, task, subtask

#let style-number(number) = text(black)[#number |]

#show raw.where(block: true):  it => grid(
  columns: 2,
  align: (right, left),
  gutter: 0.5em,
  ..it.lines
    .enumerate()
    .map(((i, line)) => (style-number(i + 1), line))
    .flatten(),
)

#show raw.where(block: true): block.with(
  fill: rgb(225, 200, 245),
  inset: 4pt,
  radius: 2pt,
)


#show: project.with(
    title: "ПЗ ТВИМС с Виолеттой",
    
    university: [MAI],
    institute: [Robotics and Intelligent Systems],
    seminar: [HTB Academy],
    
    text-font: "Segoe Print",
    abstract: [
      
      *Цель*: сдать экз. 
    ],
    show-outline: true,
      
    author: "Platon Nikitin",
    
    show-solutions: false
)

#pagebreak()
= ПЗ №2. Условные распределения двумерного непрерывного случайного вектора $(X,Y)^T$

#rect[
  #grid(
    columns: 4,
    align: center,
    inset: (x: 2pt, y: 5pt),
    [*`X\Y`*], [*$-1$*], [*$0$*], [*$1$*],
    [*$-1$*], [$1/6$], [$1/12$], [$7/24$],
    [*$1$*], [$1/8$], [$1/3$], [$0$]
  )
]

$psi(x) = M[Y|X=x]$


#task[Частные распределения, вроде][
  $X=x_1=-1$\
  $P(Y=-1|X=-1)=(1/6)/(1/6+1/12+7/24)=(1/6)/(13/24) = 4/13$

  $P(Y=0|X=-1)=(1/12)/(13/24)=2/13$

  $P(Y=1|X=-1)=(7/24)/(13/24)=7/13$

  $
    (Y|X=-1) ~ mat(
      -1, 0, 1;
      4/13, 2/13, 7/13;
    )
  $

  $psi(x_1)=-1 dot.op 4/13 + 0 + 1 dot.op 7/13 = 3/13$

  #line(length: 100%)

  $X=x_2=1$

  $P(Y=-1|X=1)=(1/8)/(1/7+1/3)=3/11$

  $P(Y=0|X=1)=(1/3)/(11/24)=8/11$

  $P(Y=1|X=1)=0$

  $
    (Y|X=1) ~ mat(
      -1, 0, 1;
      3/11, 8/11, 0;
    )
  $

  $psi(x_2) = -1 dot.op 3/11 + 0 + 0 = -3/11$
]
#line(length: 100%)

#pagebreak()
#task[$phi(Y), psi(X)$][
  $ phi(Y) = M[X|Y], psi(X) = M[Y|X] $
  $phi(Y) = cases(
      -1/7 ", " & Y=-1,
      3/5 ", " & Y=0,
      -1 ", " & Y=1,
    )
  $

  $phi(Y) ~ mat(
      -1, -1/7, 3/5;
      7/24, 7/24, 10/24;
    )
  $

  $psi(X) = cases(
      3/13 ", " & X=-1,
      -3/11 ", " & X=1,
    )
  $

  $psi(X) ~ mat(
      -3/11, 3/13;
      11/14, 13/24;
    )
  $
]
#line(length: 100%)

#task[$M[phi(Y)], M[psi(X)]$][
  $ M[phi(Y)]=M[M[X|Y]]=M[X] $
  $M[phi(Y)]=(-1) dot.op 7/24 + (-1/7) dot.op 7/24 + 3/5 dot.op 10/24 = -1/12 = M[X]$

  $M[psi(X)]=M[M[Y|X]]=M[Y]$

  $M[psi(X)]=-3/11 dot.op 11/24 + 3/13 dot.op 13/24 = 0 = M[Y] $
]
#task[ 4 значения:][
  $F(0.3|Y=0) = P(X<=0.3; Y=0)/P(Y=0)=(1/12)/(1/12+1/3)=1/5$

  $P(X in [-4; 0.6]|Y=-1)=P(X in [-4;0.6]; Y=-1])/P(Y=-1)=(1/6)/(1/6+1/8)=(1/6)/(14/48)=(1/6)/(7/24)=1/6 dot.op 24/7 = 4/7$

  $F(-0.4|X=1)=P(X<=-0.4;X=1)/P(X=1)=(1/8)/(1/8+1/3)=(1/8)/(11/24)=3/11$

  $P(Y in [-0.5; 7]|X=-1)=P(Y in [-0.5; 7]; X=-1)/P(X=-1)=9/13$
]

#line(length:100%)
#pagebreak()
*УСЛОВНАЯ Ф-ЦИЯ РАСПРЕДЕЛЕНИЯ*\
СВ $X$ при условии, что СВ $Y=y$:
$ F_X (X|Y=y) = P(X<=x|Y=y) = integral_(-oo)^(x) f_x (t|Y=y) d t $

*УСЛОВНАЯ ПЛОТНОСТЬ ВЕРОЯТНОСТИ*\
СВ $X$ при условии, что СВ $Y=y$:
$ f_x (x|Y=y) = cases(
  f(x,y)/(f_Y (y)) ", если" & f_Y (y) > 0,
  0 ", если" & f_Y (y) = 0
) $

$F_X (x|Y=y)$ и $f_X (x|Y=y)$ Рассматриваются как функции аргумента $x$,
переменная  $y$ выступает в качестве параметра.
При этом $ P(X in B|Y=y) = integral_(B) f_x (x|y=y) d x $

*УСЛОВНОЕ МАТЕМАТИЧЕСКОЕ ОЖИДАНИЕ*\
СВ $X$ при условии, что $Y=y$:
$ M[X|Y=y]=phi(y)=integral_(-oo)^(+oo) x f_X (x|Y=y) d x $
$ phi(Y) = M[X|Y] $

В случае зависимых случайных величин $X,Y$:
$ f(x,y) = f_X (x) f_Y (y|X=x)= f_Y f_X (x|Y=y)$

В случае независимых случайных величин $X,Y$:
$ f_X (x|Y=y) f_X (x)$
$ f_Y (y|X=x) f_Y (y)$

При этом $M[X|Y] = M[X], M[Y|X]=M[Y]$


#task[По плану исследовать двумерный случ. вектор $(X,Y)^T$, распределенный равномерно в ромбе - $|x|+|y|<=1$][
  Для равномерно распределенного случайного вектора $f(x,y)$ имеет вид:
  $ f(x,y) = cases(
    1/S ", если" & (x,y) in M,
    0 ", если" & (x, y) not in M
  ) $
  где M -- область, на которой СВ определен.

  $f(x,y) = cases(
    1/2 ", " & |x|+|y|<=1,
    0 ", " & |x|+|y|>1,
  )$

  #subtask[Частные плотности вероятности, M[X], D[X]]
  $ f_X (x) = integral_(-oo)^(oo) f(x,y) d y $
  $ f_Y (y) = integral_(-oo)^(oo) f(x,y) d x $
  
  $f_X(x) = integral_(-oo)^(oo) f(x,y) d y = cases(
    0 ", если" & |x|>1,
    integral_(-x-1)^(x+1) 1/2 d y 1/2 y |_(-x-1)^(x+1) = 1/2 (x+1+x+1) =
    x+1 ", если" & x in [-1;0],
    
    integral_(x-1)^(-x+1) 1/2 d y = 1/2 y |_(x-1)^(-x+1) = 1/2 (-x+1-x+1) =
    -x+1 ", если" & x in [-0;1],
  ) $

  $M[X] = integral_(-oo)^(oo) x f_X (x) d x = integral_(-1)^(x-1) x (x+1) d x + integral_0^(+1) x (-x+1) d x = integral_(-1)^0 (x^2 + x) d x + integral_0^1 (-x^2 + x) d x = (x^3)/3 |_1^0 (x^2)/2 |_1^0 - (x^3)/3 |_0^1 + (x^2)/2 |_0^1 = +1/3 -1/2 -1/3 +1/2 = 0$

  $M[X^2] = 1/6 ("Очевидно...")$
]

Домашка: Продолжить прошлую домашку до конца
