# SequenceOTRizer

## Usage

### Quickstart
Download the corresponding archive from 
[the releases page](https://github.com/leggettc18/SequenceOTRizer/releases),
extract it to your SoH folder, and run the executable in the command
line/terminal with the following arguments.
```
SequenceOTRizer --seq-path <custom music folder>
```
If you are on Windows, you may need to run SequenceOTRizer.exe. If you
are on a Mac, you can run this anywhere and supply the path to your SoH
install for the `--game-path` parameter. It should be:
```
SequenceOTRizer --seq-path <seq folder> --game-path "$HOME/Library/Application Support/com.shipofharkinian.soh"
```
### Advanced
#### Parameters
Parameter &nbsp; &nbsp; &nbsp; &nbsp; | Required? | Description
---|---|---
`--seq-path` | yes | The path to a parent folder for your custom sequences and corresponding .meta files. It will be searched recursively.
`--game-path` | no | The path to your Ship of Harkinian executable file. A `mods` folder will be created here with the output OTR inside of it. It defaults to the current working directory.
`--otr-name` | no | The name of the output OTR file _without_ the .otr extension. That will be added automatically. Defaults to "custom-music".