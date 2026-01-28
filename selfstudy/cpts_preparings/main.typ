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
    title: "ROAD TO BECOME A HACKER MASTERMIND",
    
    university: [MAI],
    institute: [Robotics and Intelligent Systems],
    seminar: [HTB Academy],
    
    text-font: "Segoe Print",
    abstract: [
      Данная часть репозитория направлена на подготовку
      к сдаче экзамена CPTS, lets go мамкины хацкеры...

      #align(horizon)[
        #figure(
          image("assets/cpts-logo3.png", width: 100%),
        )
      ]
        
      *Цель*: сдать экз. +получить первые навыки OffSec 
    ],
    show-outline: true,
      
    author: "Platon Nikitin",
    
    show-solutions: false
)

#pagebreak()
= Road to Red Team. CPTS is first step

= Why?

Здесь содержатся все заметки и записи по курсам HTB Penetration Tester Path.

Основная цель это обрести знания, навыки и оплачиваемую работу в интересующем меня направлении. 

Это не значит, что подготовка к ШАД отменяется.


#pagebreak()
= Chapter 0: Introduction


== Penetration Testing Process
#include("penetration_testing_process.typ")

#pagebreak()
== Getting Started


#pagebreak()
= Chapter 1: CReconnaissance, Enumeration & Attack Planning

== Network Enumeration with Nmap
#include("network_nmap.typ")

