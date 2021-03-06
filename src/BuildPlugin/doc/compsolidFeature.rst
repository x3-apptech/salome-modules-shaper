.. |feature_compsolid.icon|    image:: images/feature_compsolid.png

CompSolid
=========

Compsolid feature creates a compsolid using shapes already existing in other objects.

To create a compsolid in the active part:

#. select in the Main Menu *Build - > CompSolid* item  or
#. click |feature_compsolid.icon| **CompSolid** button in the toolbar

The following property panel will be opened:

.. figure:: images/Compsolid.png
  :align: center

  Create a compsolid

Select one or several shapes in the viewer.

**Apply** button creates a compsolid.

**Cancel** button cancels the operation. 

**TUI Command**:

.. py:function:: model.addCompSolid(Part_doc, Shapes)

    :param part: The current part object.
    :param list: A list of shapes.
    :return: Result object.

Result
""""""

The result of the operation will be a compsolid created from the selected shapes:

.. figure:: images/CreateCompsolid.png
  :align: center

  Result of the operation.

**See Also** a sample TUI Script of :ref:`tui_create_compsolid` operation.
