

ExpandableExmpl.chm : *.htm ExpandableExmpl.hhp ExpandableExmpl.hhc App.css
  -1 "C:\Program Files (x86)\HTML Help Workshop\hhc" ExpandableExmpl


Clean.cln    :
  @echo "Cleaning"
  cd $(MAKEDIR)
  if exist ExpandableExmpl.chm del /F ExpandableExmpl.chm


Release/ExpandableExmpl.chm : ExpandableExmpl.chm
  copy $(MAKEDIR)\ExpandableExmpl.chm  $(MAKEDIR)\..\Release\ExpandableExmpl.chm


Release/Clean.cln    : Clean.cln
  cd $(MAKEDIR)\..\Release
  if exist ExpandableExmpl.chm del /F ExpandableExmpl.chm
  cd $(MAKEDIR)


Debug/ExpandableExmpl.chm : ExpandableExmpl.chm
  copy $(MAKEDIR)\ExpandableExmpl.chm  $(MAKEDIR)\..\Debug\ExpandableExmpl.chm


Debug/Clean.cln    : Clean.cln
  cd $(MAKEDIR)\..\Debug
  if exist ExpandableExmpl.chm del /F ExpandableExmpl.chm
  cd $(MAKEDIR)


Release/Rebuild.rbd : Release/Clean.cln Release/ExpandableExmpl.chm


Debug/Rebuild.rbd   : Debug/Clean.cln Debug/ExpandableExmpl.chm


