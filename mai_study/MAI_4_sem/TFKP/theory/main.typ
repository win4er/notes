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

#set table(
  stroke: none,
)


#show: project.with(
    title: "Теория Функции Комплексного Переменного (ТФКП)",
    
    university: [MAI],
    institute: [Robotics and Intelligent Systems],
    seminar: [TFKP lectures],
    
    text-font: "Segoe Print",
    abstract: [
      *Цель:* сдать экзамен и поднять понимание.
    ],
    show-outline: true,
      
    author: "Platon Nikitin",
    
    show-solutions: false
)

#pagebreak()
= Лекция №1. Введение в ТФКП.

*КОМПЛЕКСНЫЕ ЧИСЛА*\
-- это множество чисел $CC$ являющееся расширением множества действительных
чисел $RR$.

Вообще впервые про эти числа зашла речь из-за формулы *Кардано*
(ф-ла решения кубических уравнений).

Но на лекции мы обсжудали следующее:\
$ NN = {1, 2, 3, ...}$\
$ ZZ = {0, plus.minus 1, plus.minus 2, ... }$\
$ QQ = {m/n | m in ZZ, n in NN}$\
$ R = (-oo, +oo)$\

#task[Но как же решать x^2 + 1 = 0?][
  Тут-то и появляются компл. числа
  $ CC = {(x, y) | x in RR, y in RR} $
  причем $i^2 = -1, i = sqrt(-1)$,
  $ x^2 = -1, => x = plus.minus i$
]
Операции над _комплексными числами_:

$ (x_1, y_1) plus.minus (x_2, y_2) = (x_1 plus.minus x_2, y_1 plus.minus y_2) $
$ (x_1, y_1)*(x_2, y_2) = (x_1^x_2 - y_1 * y_2, x_1 * y_2 + y_1 * x_2) $

== Алгебраическая форма записи числа:

$ z = a + i b, a in RR, b in RR $

== Показательная (экспоненциальная) форма записи числа:

$ z = |r| e^(i phi(t)) $

Не забываем формулу Эйлера:
$ e^(i phi) = cos(phi) + i sin(phi) $

И ее интерпретация через ряды Тейлора.

== Операция сопряжения.

#task[Вопрос как решать такое? $ z_1 / z_2 = (a + i b) / (c + i d) $][
  $
  z_1 / z_2 = (a + i b) / (c + i d) = 
  (a + i b) / (c + i d) * (c - i d) / (c - i d) = 
  ((a + i b) dot.op (c - i d))/ (c^2 + d^2)
  $
]

Выводится через *формулу Муавра*


== Аргумент и Модуль комплексного числа.

*АРГУМЕНТ ЧИСЛА*\
-- это величина угла отклонения относительно оси абсцисс.

$ "Arg" z = phi + 2 pi k $

*МОДУЛЬ ЧИСЛА*\
-- это величина равная длине радиус вектора
$ |z| = a^2 + b^2, z = a + i b $

== Сопряженные числа.

Это такие компл. числа z и z', такие что у них одинаковая 
*действительная* часть, а *мнимая* часть разных знаков, но 
одного *модуля*.
#figure()[
  #image("assets/fig1.png", width: 60%)
]

#pagebreak()
== Пару слов про тригономитреческую форму записи числа

$ z = x + i y = |z|(cos(phi) + i sin(phi) $

$ z_1 dot.op z_2 = |z_1| (cos(phi_1) + i sin(phi_1)) dot.op |z_2| (cos(phi_2) + i sin(phi_2))\
= |z_1+z_2| (cos(phi_1+phi_2) + i sin(phi_1+phi_2)) $

$ z^n = z^n dot.op (cos(phi) + i sin(phi))^n = z^n dot.op (cos(n phi) + i sin(n phi)) $

=== Комплексные корни из единицы.
$ z ^ (-n) = |z| (cos((phi + 2 pi k)/n) + i sin((phi + 2 pi k)/n)) $

Ниже представлена геометрическая интерпретация корней 7 степени из 1-цы:

#figure()[
  #image("assets/fig2.png", width: 60%)
]

#pagebreak()
= Лекция №2. Функция комплексного переменного.

Говорят, что на множестве $M in CC$ _задана функция_ $f$, если
задан закон, по которому каждой точке $z in M$ ставится в соответствие
комплексное число $omega$ (конечное или бесконечное);
$ f: M -> CC  "или" omega = f(z).$

Такая функция называется *взаимно однозначной (однолистной)*, если
она преобразует различные точки $z_1, z_2 in M$ в различные, иными словами,
если из равенства $f(z_1) = f(z_2)$ следует равенство
$z_1 = z_2, forall z_1, z_2 in M$

Задание $f$ равносильно заданию двух действительных функций

$ u = u(z), v = v(z),\ u: M -> RR; v: M -> RR\
(z=x+i y f(z)=u+i v, f != 0, != infinity^2) $

То полагая $ f(z) = p e^(i phi) $, мы можем записать эту
функцию в виде двух соотношений:
$ p=p(z), phi=phi(z)+2 pi k (k=0, +-1, ...) $

В точках, где $f = 0$, функция $p=0$ или $=oo$, а $phi$ не определена.

#line(length: 100%)

Пусть функция $f$ определена в проколотой окрестности точки $a in CC$;
говорят, что число $A in CC$ является ее пределом при z, стремящемся к $a$,

$ lim_(z->a) f(z) = A, $

если для любой окрестности $U_A$ точки $A$ найдется такая проколотая
окрестность $U_a '$, что для всех $z in U_a '$ значения $f(z)$ 
принадлежат $U_A$. Иначе, для любого $ epsilon >0$ найдется $sigma>0$ такое,
что из неравенства

$ 0<p(z,a)<sigma $

следует неравенство

$ p(f(z), A) < epsilon. $

И если A не $oo$, то 

$ 0 < |z-a| < sigma, |f(z)-A|<epsilon $

#line(length: 100%)

Вернемся к лк ...

$w = f(z) = u(x,y) + i v (x,y)$

== Элементарные функции КП.

$ e^z = Sigma_(i=0)^oo z!/i!, $
$ e^(i phi) = 1 + i phi + ((i z)^2)/2! + ... + ((i z)^n)/n! + ... = $
$ = 1 + i phi - (phi^2)/2 - (i phi^3)/3! + (phi^4)/4! - ... + ((i phi)^n) / n! $
$ = cos(phi) + i sin(phi) $


