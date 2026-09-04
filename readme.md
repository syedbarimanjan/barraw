# A paint app written in c compiled to wasm.
This is a paint app written in c + raylib where you can create rectangle, circles, lines and pen lines, you can also select these shapes, delete them, zoom in/out of the artboard and move the artboard around. You can also resize the shapes by holding thier edge lines(this is a little harder because of bad ux but still works).

**Shortcuts :**\
**r** = rectangle\
**c** = circle\
**l** = line\
**p** = pen\
**v** = selection\
**a** = clear artboard\
**q** = multiple selection\
**after selecting shape and clicking d** = delete shape\
**scroll wheel up/down** = zoom\
**scroll wheel click and move** = move artboard

# Preview
![game](https://cdn.hackclub.com/01a06d2a-28e8-706e-9b05-93f38dcae504/screenshot_20260901_053111.png)

# How to run locally

Install raylib on your system using this wiki https://github.com/raysan5/raylib/wiki

Install make

Clone this repo and run 
```
make main
./main
```

### AI DISCLOSURE
**No LLM/AI was used in the making of this project.**

## Todo
1.  [x] create state to store shapes info
2.  [x] functionality to select shapes
3.  [x] functionality to move shapes
4.  [x] functionality to dynamically resize shapes
5.  [x] functionality to delete shapes
6.  [x] functionality to copy shapes
7.  [x] functionality to select all shapes/ drag selection tool and everything under the rec gets selected.
8.  [x] create a tool panel
9.  [ ] create a color palette
10. [ ] functionality to change width size of shapes
11. [ ] functionality to add fill color in shapes
12. [ ] functionality to add images 
13. [ ] create a text tool
14. [ ] create an eraser
15. [ ] create state to store layer info
16. [ ] add z index


##### Todo Long Term
1.  [ ] create a plugin system

###### Todo Architecture experiments
1.  [ ] make the whole app be plugins and any optional code also a plugin
