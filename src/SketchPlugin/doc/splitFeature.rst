.. |split.icon|    image:: images/Split.png

Split
=====

| The Split operation splits sketch curve into multiple segments.
| The curve should have points lying on it.
| Open curves (line or arc) require one or more points to split with; closed curves (circle) require two or more points.

To create a Split in the active Sketch:

#. select in the Main Menu *Sketch - > Split* item  or
#. click |split.icon| **Split** button in Sketch toolbar:

Property panel:

.. figure:: images/Split_panel_Sketch.png
   :align: center

   Split

Input fields:

- **Segment** is used to select a segment to be split off in the view.

The selected segment is highlighted in the view:

.. figure:: images/Split_segment_sel.png
   :align: center

   The segment to split off

**TUI Command**:

.. py:function:: Sketch_1.addSplit(Feature, PositionPoint)

    :param object: Line, arc or circle.
    :param object: Position point (a point on the segment to split off or closest to it).
    :return: Result object.

Result
""""""

Created Split appears in the view.

.. figure:: images/Split_res.png
   :align: center

   Created split 

**See Also** a sample TUI Script of :ref:`tui_create_split` operation.
