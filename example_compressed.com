!<<bel>>=
<174>=   !check each instance of 0; if i[0]+1 has any other chars anywhere, move on to the next link
<175>=<174>   [!since there was no instance of "<174> " without there being "<174>  " but there are multiple "<174>   ["
<176>=13.
<177>=<176>28.
<178>=<176>29.
=<<bel>>!\n
LOG:
      [13.28.18] Hello   World!
      [13.28.43]   Ola!
      [13.29.01] Lol
      [13.29.29] Why   does   this   not   work?
