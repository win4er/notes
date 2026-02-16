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
