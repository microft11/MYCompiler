# MYCompiler
    docker有些情况目录是挂载不上的，多发现于轻薄本
    可以详见我再命令txt中描述的解决方式。

    利用了flex和bison
    Flex 用来描述 EBNF 中的终结符部分, 也就是描述 token 的形式和种类. 
    Bison 用来描述 EBNF 本身, 其依赖于 Flex 中的终结符描述. 它会生成一个 LALR parser.