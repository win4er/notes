#import "@preview/grape-suite:3.1.0": exercise
#import exercise: project, task, subtask

#show: project.with(
    title: "Computer Architecture and Operating Systems",

    university: [MAI],
    institute: [Robotics and Intelligent Systems],
    seminar: [CAOS notes],

    abstract: [
      Здесь собраны записи по книге Таненбаума, а также курсу АКОС МФТИ.\
      *Цель*: получить теоретические и практические знания в мире low-level
      программирования, а также
    ],
    show-outline: true,

    author: "Platon Nikitin",

    show-solutions: false
)
#pagebreak()

= *Chapter I:* Introduction

#include("chapter-1.typ")

#pagebreak()
= *Chapter II:* Computer System Organization

#pagebreak()
= *Chapter III:* Digital logic level

#pagebreak()
= *Chapter IV:* Microarchitecutre level

#pagebreak()
= *Chapter V:* Level of architecture command execution

#pagebreak()
= *Chapter VI:* Operating System level

#pagebreak()
= *Chapter VII:* Assembly level

#pagebreak()
= *Chapter VIII:* Parallel Computer Architectures

#pagebreak()
= *Chapter IX:* Bibliography
