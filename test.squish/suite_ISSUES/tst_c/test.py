def main():
    source(findFile("scripts", "common.py"))
    
    startApplication("linux_run.sh")
    set_defaults()
    
    clickButton(waitForObject(":Operations.New part_AppElements_Button"))
    clickTab(waitForObject(":General.qt_tabwidget_tabbar_QTabBar"), "Sketch")
    clickButton(waitForObject(":Basic.Sketch_AppElements_Button"))
    mouseClick(waitForObject(":OpenParts*_AppElements_ViewPort"), 351, 267, 0, Qt.LeftButton)
    clickButton(waitForObject(":Basic.Line_AppElements_Button"))
    mouseClick(waitForObject(":OpenParts*_AppElements_ViewPort"), 242, 169, 0, Qt.LeftButton)
    mouseClick(waitForObject(":OpenParts*_AppElements_ViewPort"), 140, 394, 0, Qt.LeftButton)
    mouseClick(waitForObject(":OpenParts*_AppElements_ViewPort"), 531, 537, 0, Qt.LeftButton)
    mouseClick(waitForObject(":OpenParts*_AppElements_ViewPort"), 241, 169, 0, Qt.LeftButton)
    clickButton(waitForObject(":Sketch.property_panel_ok_QToolButton"))
    clickTab(waitForObject(":General.qt_tabwidget_tabbar_QTabBar"), "Features")
    mouseDrag(waitForObject(":Features_QScrollArea"), 31, 1, -2, -2, 1, Qt.LeftButton)
    clickButton(waitForObject(":Extrusion.Extrusion_AppElements_Button"))
    mouseClick(waitForObject(":OpenParts*_AppElements_ViewPort"), 216, 270, 0, Qt.LeftButton)
    clickButton(waitForObject(":Extrusion.property_panel_ok_QToolButton"))
    clickButton(waitForObject(":Extrusion.ExtrusionCut_AppElements_Button"))
    clickButton(waitForObject(":Basic_QToolButton"))
    clickButton(waitForObject(":Abort operation.OK_QPushButton"))
    clickButton(waitForObject(":Application errors.Close_QPushButton"))
    clickButton(waitForObject(":Extrusion.ExtrusionFuse_AppElements_Button"))
    mouseClick(waitForObject(":OpenParts*_AppElements_ViewPort"), 290, 316, 0, Qt.LeftButton)
    mouseClick(waitForObject(":Sketch.Basic_AppElements_MenuGroupPanel"), 143, 56, 0, Qt.LeftButton)
    clickButton(waitForObject(":Basic.Circle_AppElements_Button"))
    mouseClick(waitForObject(":OpenParts*_AppElements_ViewPort"), 259, 248, 0, Qt.LeftButton)
    mouseClick(waitForObject(":OpenParts*_AppElements_ViewPort"), 281, 285, 0, Qt.LeftButton)
    clickButton(waitForObject(":Sketch.Ok_QToolButton"))
    mouseClick(waitForObject(":Extrusion.qt_spinbox_lineedit_QLineEdit_5"), 42, 9, 0, Qt.LeftButton)
    type(waitForObject(":Extrusion.to_size_ModuleBase_ParamSpinBox_2"), "<Ctrl+A>")
    type(waitForObject(":Extrusion.to_size_ModuleBase_ParamSpinBox_2"), "3")
    type(waitForObject(":Extrusion.to_size_ModuleBase_ParamSpinBox_2"), "3")
    mouseClick(waitForObject(":Extrusion.qt_spinbox_lineedit_QLineEdit_6"), 93, 7, 0, Qt.LeftButton)
    type(waitForObject(":Extrusion.from_size_ModuleBase_ParamSpinBox_3"), "<Ctrl+A>")
    type(waitForObject(":Extrusion.from_size_ModuleBase_ParamSpinBox_3"), "4")
    type(waitForObject(":Extrusion.from_size_ModuleBase_ParamSpinBox_3"), "4")
    clickButton(waitForObject(":ExtrusionFuse.property_panel_ok_QToolButton"))
    clickButton(waitForObject(":Operations.New part_AppElements_Button"))
    clickTab(waitForObject(":General.qt_tabwidget_tabbar_QTabBar"), "Sketch")
    clickButton(waitForObject(":Basic.Sketch_AppElements_Button"))
    mouseClick(waitForObject(":OpenParts*_AppElements_ViewPort"), 351, 267, 0, Qt.LeftButton)
    clickButton(waitForObject(":Basic.Line_AppElements_Button"))
    mouseClick(waitForObject(":OpenParts*_AppElements_ViewPort"), 242, 169, 0, Qt.LeftButton)
    mouseClick(waitForObject(":OpenParts*_AppElements_ViewPort"), 140, 394, 0, Qt.LeftButton)
    mouseClick(waitForObject(":OpenParts*_AppElements_ViewPort"), 531, 537, 0, Qt.LeftButton)
    mouseClick(waitForObject(":OpenParts*_AppElements_ViewPort"), 241, 169, 0, Qt.LeftButton)
    clickButton(waitForObject(":Sketch.property_panel_ok_QToolButton"))
    clickTab(waitForObject(":General.qt_tabwidget_tabbar_QTabBar"), "Features")
    mouseDrag(waitForObject(":Features_QScrollArea"), 31, 1, -2, -2, 1, Qt.LeftButton)
    clickButton(waitForObject(":Extrusion.Extrusion_AppElements_Button"))
    mouseClick(waitForObject(":OpenParts*_AppElements_ViewPort"), 216, 270, 0, Qt.LeftButton)
    clickButton(waitForObject(":Extrusion.property_panel_ok_QToolButton"))
    clickButton(waitForObject(":Extrusion.ExtrusionCut_AppElements_Button"))
    clickButton(waitForObject(":Basic_QToolButton"))
    clickButton(waitForObject(":Abort operation.OK_QPushButton"))
    clickButton(waitForObject(":Application errors.Close_QPushButton"))
    clickButton(waitForObject(":Extrusion.ExtrusionFuse_AppElements_Button"))
    mouseClick(waitForObject(":OpenParts*_AppElements_ViewPort"), 290, 316, 0, Qt.LeftButton)
    mouseClick(waitForObject(":Sketch.Basic_AppElements_MenuGroupPanel"), 143, 56, 0, Qt.LeftButton)
    clickButton(waitForObject(":Basic.Circle_AppElements_Button"))
    mouseClick(waitForObject(":OpenParts*_AppElements_ViewPort"), 259, 248, 0, Qt.LeftButton)
    mouseClick(waitForObject(":OpenParts*_AppElements_ViewPort"), 281, 285, 0, Qt.LeftButton)
    clickButton(waitForObject(":Sketch.Ok_QToolButton"))
    mouseClick(waitForObject(":Extrusion.qt_spinbox_lineedit_QLineEdit_5"), 42, 9, 0, Qt.LeftButton)
    type(waitForObject(":Extrusion.to_size_ModuleBase_ParamSpinBox_2"), "<Ctrl+A>")
    type(waitForObject(":Extrusion.to_size_ModuleBase_ParamSpinBox_2"), "3")
    type(waitForObject(":Extrusion.to_size_ModuleBase_ParamSpinBox_2"), "3")
    mouseClick(waitForObject(":Extrusion.qt_spinbox_lineedit_QLineEdit_6"), 93, 7, 0, Qt.LeftButton)
    type(waitForObject(":Extrusion.from_size_ModuleBase_ParamSpinBox_3"), "<Ctrl+A>")
    type(waitForObject(":Extrusion.from_size_ModuleBase_ParamSpinBox_3"), "4")
    type(waitForObject(":Extrusion.from_size_ModuleBase_ParamSpinBox_3"), "4")
    clickButton(waitForObject(":ExtrusionFuse.property_panel_ok_QToolButton"))
    i=1
    
    sendEvent("QCloseEvent", waitForObject(":OpenParts*_AppElements_MainWindow"))
