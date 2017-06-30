// Copyright (C) 2014-2017  CEA/DEN, EDF R&D
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
//
// See http://www.salome-platform.org/ or
// email : webmaster.salome@opencascade.com<mailto:webmaster.salome@opencascade.com>
//

#include "GeomAlgoAPI_Scale.h"

#include <BRepBuilderAPI_Transform.hxx>
#include <BRepBuilderAPI_GTransform.hxx>

//=================================================================================================
GeomAlgoAPI_Scale::GeomAlgoAPI_Scale(std::shared_ptr<GeomAPI_Shape> theSourceShape,
                                     std::shared_ptr<GeomAPI_Pnt>   theCenterPoint,
                                     double theScaleFactor)
{
  myMethodType = BY_FACTOR;
  mySourceShape = theSourceShape;
  myCenterPoint = theCenterPoint;
  myScaleFactor = theScaleFactor;
}

//=================================================================================================
GeomAlgoAPI_Scale::GeomAlgoAPI_Scale(std::shared_ptr<GeomAPI_Shape> theSourceShape,
                                     std::shared_ptr<GeomAPI_Pnt>   theCenterPoint,
                                     double theScaleFactorX,
                                     double theScaleFactorY,
                                     double theScaleFactorZ)
{
  myMethodType = BY_DIMENSIONS;
  mySourceShape = theSourceShape;
  myCenterPoint = theCenterPoint;
  myScaleFactorX = theScaleFactorX;
  myScaleFactorY = theScaleFactorY;
  myScaleFactorZ = theScaleFactorZ;
}

//=================================================================================================
bool GeomAlgoAPI_Scale::check()
{
  if (!mySourceShape) {
    myError = "Scale builder :: source shape is not valid.";
    return false;
  }
  if (!myCenterPoint) {
    myError = "Scale builder :: center point is not valid.";
    return false;
  }
  switch (myMethodType) {
    case BY_FACTOR: {
      if (fabs(myScaleFactor) < Precision::Confusion()) {
        myError = "Scale builder :: the scale factor is null.";
        return false;
      }
      return true;
    }
    case BY_DIMENSIONS: {
      if (fabs(myScaleFactorX) < Precision::Confusion()) {
        myError = "Scale builder :: the scale factor in X is null.";
        return false;
      }
      if (fabs(myScaleFactorY) < Precision::Confusion()) {
        myError = "Scale builder :: the scale factor in Y is null.";
        return false;
      }
      if (fabs(myScaleFactorZ) < Precision::Confusion()) {
        myError = "Scale builder :: the scale factor in Z is null.";
        return false;
      }
      return true;
    }
    default: {
      myError = "Scale builder :: method not implemented.";
      return false;
    }
  }
}

//=================================================================================================
void GeomAlgoAPI_Scale::build()
{
  switch (myMethodType) {
   case BY_FACTOR : {
     buildByFactor();
     break;
   }
   case BY_DIMENSIONS : {
     buildByDimensions();
     break;
   }
   default : {
     myError = "Scale builder :: method not yet implemented";
     return;
   }
  }
}

//=================================================================================================
void GeomAlgoAPI_Scale::buildByFactor()
{
  const gp_Pnt& aCenterPoint = myCenterPoint->impl<gp_Pnt>();
  gp_Trsf* aTrsf = new gp_Trsf();
  aTrsf->SetScale(aCenterPoint, myScaleFactor);

  const TopoDS_Shape& aSourceShape = mySourceShape->impl<TopoDS_Shape>();

  if(aSourceShape.IsNull()) {
    myError = "Scale builder :: source shape does not contain any actual shape.";
    return;
  }

  // Transform the shape while copying it.
  BRepBuilderAPI_Transform* aBuilder = new BRepBuilderAPI_Transform(aSourceShape, *aTrsf, true);
  if(!aBuilder) {
    myError = "Scale builder :: transform initialization failed.";
    return;
  }

  setImpl(aBuilder);
  setBuilderType(OCCT_BRepBuilderAPI_MakeShape);

  if(!aBuilder->IsDone()) {
    myError = "Scale builder :: algorithm failed.";
    return;
  }

  TopoDS_Shape aResult = aBuilder->Shape();

  std::shared_ptr<GeomAPI_Shape> aShape(new GeomAPI_Shape());
  aShape->setImpl(new TopoDS_Shape(aResult));
  setShape(aShape);
  setDone(true);
}

//=================================================================================================
void GeomAlgoAPI_Scale::buildByDimensions()
{
  const gp_Pnt& aCenterPoint = myCenterPoint->impl<gp_Pnt>();

  // Perform the rotation matrix
  gp_Mat aMatRot(myScaleFactorX, 0., 0.,
                 0., myScaleFactorY, 0.,
                 0., 0., myScaleFactorZ);

  // Perform the tranformation
  gp_Pnt anOriginPnt(0., 0., 0.);
  gp_GTrsf aGTrsf;
  gp_GTrsf aGTrsfP0;
  gp_GTrsf aGTrsf0P;
  aGTrsfP0.SetTranslationPart(anOriginPnt.XYZ() - aCenterPoint.XYZ());
  aGTrsf0P.SetTranslationPart(aCenterPoint.XYZ());
  aGTrsf.SetVectorialPart(aMatRot);
  aGTrsf = aGTrsf0P.Multiplied(aGTrsf);
  aGTrsf = aGTrsf.Multiplied(aGTrsfP0);

  const TopoDS_Shape& aSourceShape = mySourceShape->impl<TopoDS_Shape>();

  if(aSourceShape.IsNull()) {
    myError = "Scale builder :: source shape does not contain any actual shape.";
    return;
  }

  // Transform the shape while copying it.
  BRepBuilderAPI_GTransform* aBuilder = new BRepBuilderAPI_GTransform(aSourceShape, aGTrsf, true);
  if(!aBuilder) {
    myError = "Scale builder :: transform initialization failed.";
    return;
  }

  setImpl(aBuilder);
  setBuilderType(OCCT_BRepBuilderAPI_MakeShape);

  if(!aBuilder->IsDone()) {
    myError = "Scale builder :: algorithm failed.";
    return;
  }

  TopoDS_Shape aResult = aBuilder->Shape();

  std::shared_ptr<GeomAPI_Shape> aShape(new GeomAPI_Shape());
  aShape->setImpl(new TopoDS_Shape(aResult));
  setShape(aShape);
  setDone(true);
}
