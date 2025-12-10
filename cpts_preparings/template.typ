// Тёмная тема в стиле Markdown
#let dark-theme = (
  background: rgb("#101014"),
  text: rgb("#e0e0e0"),
  accent: rgb("#58a6ff"),
  secondary: rgb("#8b949e"),
  border: rgb("#30363d"),
  code-bg: rgb("#2d2d2d")
)

// Горизонтальная линия (как --- в Markdown)
#let hr = line(
  stroke: (paint: dark-theme.border, thickness: 1pt),
  length: 100%,
)

// Ссылки
#show link: it => [
  #set text(fill: dark-theme.accent, underline: true)
  #it
]

#let template = doc => {
  // Базовые настройки
  set page(
    paper: "a4",
    margin: (top: 2.5cm, bottom: 2cm, left: 2cm, right: 2cm),
    fill: dark-theme.background
  )

  set text(
    fill: dark-theme.text,
    font: "Hack",
    size: 11pt
  )

  // Блоки кода (как ```код``` в Markdown)
  show raw: it => [
    #set text(fill: rgb("#dcdcaa"), font: "Consolas")
    #block(
      fill: dark-theme.code-bg,
      inset: 8pt,
      radius: 0pt,
      it
    )
  ]

  // Inline код (как `код` в Markdown)
  show "`": it => {
    set text(fill: rgb("#dcdcaa"), font: "Fira Code")
    box(
      fill: dark-theme.code-bg,
      inset: (x: 3pt, y: 1pt),
      radius: 2pt,
      it
    )
  }

  // Цитаты (как > цитата в Markdown)
  show quote: it => [
    #block(
      inset: 7pt,
      stroke: (left: 2pt + dark-theme.secondary),
      fill: rgb("#202020"),
      radius: 0pt,
      it
    )
  ]

  doc
}
