# zParserExtender
## - The multiplatform plugin
Merge or override new scripts for compiled .DAT files, daedalus 'while' loop and new external functions.



## Configurations
Initialize parameters in the Gothic.ini or `YourMod`.ini:
```ini
[ZPARSE_EXTENDER]
LoadScript = Extend.d
MergeMode  = true
CompileDat = false
```
Also, these parameters will be automatically added after first launch of the plugins.

#### LoadScript
This parameter specifies a list of files that must be parsed when the game starts.

#### MergeMode
If this parameter is true, then instead of an error, new instances, functions and variables will override already existing ones.

#### CompileDat
If this parameter is true, then after adding new scripts the .DAT file will be recompiled with the new data.



## Script format
- The script is a list of files to be added. One line - one file.
The string consists of two arguments.
    1. The tag name of parser.
    2. path of the script.
- You can put this file anywhere, including a virtual volume.
- Make sure the encoding is ANSI.

#### Tags
```
- Game   or parser           - Main game parser (NPCs, AI, story and etc)
- SFX    or parserSoundFX    - Sound parser
- PFX    or parserParticleFX - Particle FX parser
- VFX    or parserVisualFX   - Visual FX parser
- Camera or parserCamera     - Camera mods parser
- Menu   or parserMenu       - Menu parser
- Music  or parserMusic      - Music themes parser
```

##### Example of the script:
```
Game	System\NewHumanAI.d
Game	System\NewItems.d
Music	System\NewMusic.d
```



## Daedalus 'while' loop

#### Syntax
```cpp
while (<expression>)
{
    // to do
    while (true)
    {
        // to do
        if (<expression>)
        {
            break;
        };
    };
};
```
