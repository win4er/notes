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
