// File:        PartSet_OperationEditLine.h
// Created:     05 May 2014
// Author:      Natalia ERMOLAEVA

#ifndef PartSet_OperationEditLine_H
#define PartSet_OperationEditLine_H

#include "PartSet.h"

#include <PartSet_OperationSketchBase.h>
#include <QObject>

class QMouseEvent;

/*!
 \class PartSet_OperationEditLine
 * \brief The operation for the sketch feature creation
*/
class PARTSET_EXPORT PartSet_OperationEditLine : public PartSet_OperationSketchBase
{
  Q_OBJECT
public:
  /// Returns the operation type key
  static std::string Type() { return "EditLine"; }

public:
  /// Constructor
  /// \param theId the feature identifier
  /// \param theParent the operation parent
  /// \param theFeature the parent feature
  PartSet_OperationEditLine(const QString& theId, QObject* theParent,
                            boost::shared_ptr<ModelAPI_Feature> theFeature);
  /// Destructor
  virtual ~PartSet_OperationEditLine();

   /// Returns that this operator can be started above already running one.
   /// The runned operation should be the sketch feature modified operation
  virtual bool isGranted() const;

  /// Returns the operation local selection mode
  /// \param theFeature the feature object to get the selection mode
  /// \return the selection mode
  virtual std::list<int> getSelectionModes(boost::shared_ptr<ModelAPI_Feature> theFeature) const;

  /// Initializes some fields accorging to the feature
  /// \param theFeature the feature
  virtual void init(boost::shared_ptr<ModelAPI_Feature> theFeature);

  /// Processes the mouse pressed in the point
  /// \param thePoint a point clicked in the viewer
  /// \param theEvent the mouse event
  virtual void mousePressed(QMouseEvent* theEvent, Handle_V3d_View theView);
  /// Gives the current mouse point in the viewer
  /// \param thePoint a point clicked in the viewer
  /// \param theEvent the mouse event
  virtual void mouseMoved(QMouseEvent* theEvent, Handle_V3d_View theView);
  /// Gives the current selected objects to be processed by the operation
  /// \param thePoint a point clicked in the viewer
  /// \param theEvent the mouse event
  /// \param theSelected the list of selected presentations
 virtual void mouseReleased(QMouseEvent* theEvent, Handle_V3d_View theView,
                             const std::list<XGUI_ViewerPrs>& theSelected);
protected:
  /// \brief Virtual method called when operation is started
  /// Virtual method called when operation started (see start() method for more description)
  /// Switch off the multi selection state
  virtual void startOperation();

  /// Virtual method called when operation stopped - committed or aborted.
  /// Restore the multi selection state
  virtual void stopOperation();

  /// Creates an operation new feature
  /// Returns NULL feature. This is an operation of edition, not creation.
  /// \returns the created feature
  virtual boost::shared_ptr<ModelAPI_Feature> createFeature();

protected:
  /// \brief Save the point to the line.
  /// \param theDeltaX the delta for X coordinate is moved
  /// \param theDeltaY the delta for Y coordinate is moved
  /// \param theAttribute the start or end attribute of the line
  void  moveLinePoint(double theDeltaX, double theDeltaY,
                      const std::string& theAttribute);

private:
  boost::shared_ptr<ModelAPI_Feature> mySketch; ///< the sketch feature
  gp_Pnt myCurPressed; ///< the current 3D point clicked or moved
};

#endif
