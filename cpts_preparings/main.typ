#import "@preview/grape-suite:3.1.0": exercise
#import exercise: project, task, subtask

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
= Road to Red Team. CPTS is first step

#pagebreak()

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

#pagebreak()
== Footprinting

#pagebreak()
== Information Gathering

#pagebreak()
== Vulnerability Assessment

#pagebreak()
== File Transfers

#pagebreak()
== Shells & Payloads

#pagebreak()
== Using the Metasploit Framework


#pagebreak()
= Chapter II: Exploitation & Lateral Movement

== Password Attacks

#pagebreak()
== Attacking Common Services

#pagebreak()
== Pivoting, Tunneling, and Port Forwarding

#pagebreak()
== Active Directory Enumeration && Attacks


#pagebreak()
= Chapter III: Web Exploitation

== Using Web Proxies

#pagebreak()
== Attacking Web Applications with Ffuf

#pagebreak()
== Login Brute Forcing

#pagebreak()
== SQL Injection Fundamentals

#pagebreak()
== Cross-Site Scripting (XSS)

#pagebreak()
== File Inclusion

#pagebreak()
== File Upload Attacks

#pagebreak()
== Command Injections

#pagebreak()
== Web Attacks

#pagebreak()
== Attacking Common Applications


#pagebreak()
= Chapter IV: Post-Exploitation

== Linux Privilege Escalation

#pagebreak()
== Windows Privilege Escalation


#pagebreak()
= Chapter V: Reporting & Capstone

== Documentation & Reporting

#pagebreak()
== Attacking Enterprise Networks
