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
    seminar: [TFKP seminars with Deniskina Galina Yurievna\ mail: `dega17@yandex.ru`],
    
    text-font: "Segoe Print",
    abstract: [
      *Цель:* сдать экзамен и поднять понимание.
    ],
    show-outline: true,
      
    author: "Platon Nikitin",
    
    show-solutions: false
)

#pagebreak()
= Семинар №1.

Небольшое повторение лекции про происхождение комплексных чисел.
Разве что надо представить ось, а затем придумать операцию перевода числа из 1
в -1.
$ x^2=-1\ x dot.op x = -1\ 1 dot.op x dot.op x = -1 $

Дальше мы упражнялись в записи чисел в разных формах и на геом. плоскости.

#figure()[
  #image("assets/fig1.png")
]

#task[Представить на геометрической плоскости][
  #subtask[$z_1 = 3$]
  #subtask[$z_2 = 2 - 2 i$]
  #subtask[$z_3 = 3$]
  #subtask[$z_4 = 3 - 2 i$]
  #subtask[$z_5 = -1 + sqrt(3) i$]
]

== Аргумент комплексного числа

$ phi = "Arg" z = arg z + 2 pi k, k in ZZ $
$ "Arg" Z := 
  cases(
    "arctg"(y/x) "," &" где" x > 0, 
    pi + "arctg" (y/x) "," &" где" x < 0 ", " y > 0,
    - pi + "arctg" (y/x) "," & " где" x < 0 ", " y < 0
  )
$

Перемножение комплексных чисел $z_1, z_2$, совершает вращение на
угол z_2 и растяжение(сжатие) $z_1$ в модуль $z_2$ раз.

#task[$z_1 = 2 + 3 i, z_2 = - 1 + 2 i, z_1 dot.op z_2, z_1 / z_2 = ?$][
  $ 
    z_1 dot.op z_2 = (2 + 3 i) dot.op (-1 + 2 i) =
    -2 -3 i + 4 i + 6 i^2 = #rect()[$ -8 + i $],\
    
    z_1 / z_2 = z_1 / z_2 dot.op bar.h(z_1) / bar.h(z_2) = 
    (2 + 3 i) / (-1 + 2 i) = (2 + 3 i) / (-1 + 2 i)
    dot.op (-1-2 i) / (-1-2 i)\ = ((2 + 3 i) (-1-2 i)) / (-1 - 4 i ^ 2) =
    ((2 + 3 i) (-1 -2 i)) / 5 = (-2 -3 i - 4 i - 6 i^2) / 5 =
    #rect()[$ (4 - 7 i) / 5 $]
  $
]

#rect()[
  *ВАЖНАЯ ФОРМУЛА:* $ z^n = |z^n| (cos(n phi) + i sin(n phi) $
]

Дальше будет формула для корней из единицы

#pagebreak()
= ПЗ №2.

$ root(n, z) = root(n, |z|) (cos (phi / n + 2 pi k) + i sin (phi / n + 2 pi k)), k in underbrace({0, +1, +2 ...}, n " штук") $

#task[Считаем корни n степени][

  $1+i = sqrt(2) (cos(pi/4)+i sin(pi/4))$

  $root(3, 1+i) = root(3,sqrt(2)) (cos(pi/4+2 pi k)/3 + i sin(pi/4+2 pi k)/3), k = 0;1;2$

  Дальше мы подставляли k и считали $w_k$, а затем построили 
  окружность на декартовой плоскости и расположили эти корни, 
  в конспекте по ЛК была данная иллюстрация.

  $ root(4, 1)=root(4, |1|) (cos (0+2 pi k)/4 +i sin (0+2 pi k)/4) =
    cases(
      1 dot.op (cos 0/4 + i sin 0/4) = 1,
      1 dot.op (cos pi/2 + i sin pi/2) = i,
      1 dot.op (cos pi + i sin pi) = -1,
      1 dot.op (cos (3 pi)/2 + i sin (3 pi)/2) = -i,
    )
  $
]

#task[$z^2-4 z + 4 - 2 i = 0$][
  $x^2 +p x + q = 0$\
  $x_(1,2) = -p/2 plus.minus sqrt((p/2)^2-q)$\
  $z_(1,2) = 2 plus.minus sqrt(4-(4-2 i)) = 2 + sqrt(2 i)$

  $sqrt(2 i) = sqrt(1^2 + 2 dot.op 1 dot.op i + i^2) = 1+i =\
  = w_o = sqrt(2) (sqrt(2)/2 + i sqrt(2)/2)\
  w_1 = sqrt(2) (cos (5 pi)/4 + i sin (5 pi)/4)
  $
]

== Мнимая и Действительная части функции ТФКП
Вообще это очевидно, но все же ...

$w = f(z) = f(x+i y)$\
$u + i v = f(x+i y)$

Пара полезных формул

#rect[
  $ cos(i z) = "ch" z $
  $ sin(i z) = i "sh" z $
  $ "ch"(i z) = cos z $
]

#pagebreak()
#task[$w=cos z = cos (x+i y)$][
  $w = cos(x+i y) = cos x dot.op cos(i y) - sin x dot.op sin(i y) = 
  cos x dot.op "ch" y - i sin x dot.op "sh" y\
  
  Re w = cos x dot.op "ch" y\
  Im w = sin x dot.op "sh" y
  $
]

#task[$w=e^z (z = 3 + pi/2)$][
  $e^(3+i pi/2) = e^3 dot.op e^(i pi/2) = e^3 dot.op i\
  e^(i pi/2) = cos pi/2 + i sin pi/2 = 0 + i\

  "ch" (i pi/4) = cos pi/4 = sqrt(2)/2
  $
]

#task[$sin(i ln 3)=$][
  $sin(i ln 3) = i "sh" (ln 3) = i (e^(ln 3)-e^(ln 3))/2
  = i (3-1/3)/2 = i (8/3)/2 = i 4/3
  $
]
