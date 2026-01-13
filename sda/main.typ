#import "@preview/grape-suite:3.1.0": exercise
#import exercise: project, task, subtask

#show: project.with(
    title: "Handbook поступающего в ШАД",
    
    university: [MAI],
    institute: [Robotics and Intelligent Systems],
    seminar: [ШАД подготовка],
  
    text-font: "Hack",
    abstract: [
      Здесь собраны записи билетов в ШАД, а также все, что
      поможет мне в поступлении в Школу Анализа Данных Яндекс.

      *Цель*: попасть в ШАД 2025-2026 годов
    ],
    show-outline: true,

    author: "Platon Nikitin",

    show-solutions: false
)

#pagebreak()

= Handbook поступающего в ШАД

= Теория вероятности

#include("prob1.typ")
#pagebreak()
#include("prob2.typ")
#pagebreak()
#include("prob3.typ")
#pagebreak()
#include("prob4.typ")
#pagebreak()
