# KSS Reference variables

KSS means Kicad Style Sheet. It is very similar to CSS syntax.

The main difference is that selectors can be regular expressions.

## Selectors

Selectors are not case sensible.

### Pin selector

Example :

```CSS
VDD {
    class: "power+";
}
```

All pins named `VDD` or `Vdd` will be putted in the class named `power+`.

|Selector|Example|Example description|
|--------|-------|-------------------|
|name|GND|All pin named GND|
|(name1\|name2)|(GND\|VSS)         |All pin named GND or VSS|
|name[12]      |VDD[AB]            |All pin named VDDA or VDDB|
|name[1-3]     |VDD[A-C]           |All pin named VDDA or VDDB or VDDC|
|name[1-3]*    |VDD[A-C]*          |All pin named VDD or VDDA or VDDB or VDDC|
|\[fb\]\[1-3\]+|\[PR\]\[A-L\][0-9]+|Match all gpio RA16, PF5, ...|

For more examples, take a look to kss files in this directory or look at regular expression reference guide.

Capture parts is possible in selector to reuse value in properties. To capture a part of text, just put it in parentheses. And to reuse it, call it with a backslash  with the id of captured part. Example :

```CSS
\[PR\]([A-L])[0-9]+ {
    class: "gpio\1";
}
```

Letter after P or R is captured and putted in `\1`. So a pin named `RB1` will be classed in class `gpioB` and `RL12` in `gpioL`.

### Class selector

Class selector are similar to pin selector but start with a point.

Example :

```CSS
.gpioA {
    position: left;
    sort: asc;
}
```

All pins classed in `.gpioA` are moved on the left of the final component with pin sorted ascended.

## Properties

At the moment, only a few set of properties are implemented.

### Pin properties

#### class [text, text with capture]

class gives the name of the name of the destination class.

If the selector contains capture(s), captured parts could be used to set the destination class.

default: "default"

#### elecType [in out bidir tri passive unspecified pin pout opcol opem nc]

Sets the electrical type of the pin.

default: in

#### pinType [norm novisible inverted clock iclock lowin clocklow lowout faledge nologic]

Sets the visual style type of the pin.

default: norm

#### priority [int]

Sets priority of the pin in class. The pin with the higher priority will be placed first in the class. The lowest, last.

Could be negative.

default: 0

### Class properties

#### position [top, bottom, left, right, aside]

position gives the position of pins on the final component. aside means left or right, depending of the residual place.

default: aside

#### sort [none, asc, desc]

If sort is different of none, sets the order of pin sorting.

default: none

#### sortpattern [regexp]

The sortpattern regular expression gives the part of pin that will be used to sort pins.

default: ".*" (the whole pin name)

#### length [int > 0]

length gives the length of pins in class. Should be comprise in [100, 300] interval with increment of 50 to respect [KLC](http://kicad-pcb.org/libraries/klc/) convention.

default: 200

#### priority [int]

Sets priority of the class. The class with the higher priority will be placed first in the side. The lowest, last.

Could be negative.

default: 0

#### visibility [visible, hidden, removed]

Visibility of the class. In case of 'hidden', all pins of the class are hidden but class is placed like others. With 'removed', pins are hidden and class is not placed.

default: visible

#### label [string]

If it set, the class add a text label with this text. Accepts matches from class selector.

default: ""

#### rect [int]

If greater than 0, draws a rect arround the class with the thikness of this value.

default: 0
