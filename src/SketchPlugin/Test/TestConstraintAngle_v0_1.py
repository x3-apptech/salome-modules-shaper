# Copyright (C) 2014-2021  CEA/DEN, EDF R&D
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2.1 of the License, or (at your option) any later version.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public
# License along with this library; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
#
# See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
#

from salome.shaper import model

model.begin()
partSet = model.moduleDocument()
Part_1 = model.addPart(partSet)
Part_1_doc = Part_1.document()
Sketch_1 = model.addSketch(Part_1_doc, model.defaultPlane("XOY"))
SketchLine_1 = Sketch_1.addLine(-17.48270627886662, -77.57247534383079, 0.4653113240225388, -68.74815303974528)
SketchLine_2 = Sketch_1.addLine(0.4653113240225388, -68.74815303974528, 8.542125432770632, -50.45156664083665)
SketchConstraintCoincidence_1 = Sketch_1.setCoincident(SketchLine_1.endPoint(), SketchLine_2.startPoint())
SketchLine_3 = Sketch_1.addLine(8.542125432770632, -50.45156664083665, -8.178735979228234, 7.171456480488104)
SketchConstraintCoincidence_2 = Sketch_1.setCoincident(SketchLine_2.endPoint(), SketchLine_3.startPoint())
SketchLine_4 = Sketch_1.addLine(-17.48270627886662, -77.57247534383079, -36.90385046950819, -72.79553507559329)
SketchConstraintCoincidence_3 = Sketch_1.setCoincident(SketchLine_1.startPoint(), SketchLine_4.startPoint())
SketchLine_5 = Sketch_1.addLine(-48.71075203912585, -56.65251567631857, -36.90385046950819, -72.79553507559329)
SketchConstraintCoincidence_4 = Sketch_1.setCoincident(SketchLine_4.endPoint(), SketchLine_5.endPoint())
SketchLine_6 = Sketch_1.addLine(-48.71075203912585, -56.65251567631857, -47.37883052420096, -36.69691533260352)
SketchConstraintCoincidence_5 = Sketch_1.setCoincident(SketchLine_5.startPoint(), SketchLine_6.startPoint())
SketchLine_7 = Sketch_1.addLine(-30.39232801419296, -18.99499937550162, -47.37883052420096, -36.69691533260352)
SketchConstraintCoincidence_6 = Sketch_1.setCoincident(SketchLine_6.endPoint(), SketchLine_7.endPoint())
SketchConstraintAngle_1 = Sketch_1.setAngle(SketchLine_1.result(), SketchLine_2.result(), 220)
SketchConstraintAngle_2 = Sketch_1.setAngle(SketchLine_3.result(), SketchLine_2.result(), 140)
SketchConstraintAngle_3 = Sketch_1.setAngle(SketchLine_1.result(), SketchLine_4.result(), 140)
SketchConstraintAngle_4 = Sketch_1.setAngle(SketchLine_6.result(), SketchLine_5.result(), 220)
SketchConstraintAngle_5 = Sketch_1.setAngle(SketchLine_5.result(), SketchLine_4.result(), 220)
SketchConstraintAngle_6 = Sketch_1.setAngle(SketchLine_6.result(), SketchLine_7.result(), 140)
SketchConstraintLength_1 = Sketch_1.setLength(SketchLine_2.result(), 20)
SketchConstraintEqual_1 = Sketch_1.setEqual(SketchLine_2.result(), SketchLine_1.result())
SketchConstraintEqual_2 = Sketch_1.setEqual(SketchLine_2.result(), SketchLine_4.result())
SketchConstraintEqual_3 = Sketch_1.setEqual(SketchLine_2.result(), SketchLine_5.result())
SketchConstraintEqual_4 = Sketch_1.setEqual(SketchLine_2.result(), SketchLine_6.result())
SketchLine_8 = Sketch_1.addLine(14.775406531722, -47.38691166633646, -20.64529817713098, 1.042146531487715)
SketchPoint_1 = Sketch_1.addPoint(2.968504962104344, -31.24389226706174)
SketchConstraintCoincidence_7 = Sketch_1.setCoincident(SketchPoint_1.coordinates(), SketchLine_3.result())
SketchConstraintCoincidence_8 = Sketch_1.setCoincident(SketchPoint_1.coordinates(), SketchLine_8.result())
SketchConstraintDistance_1 = Sketch_1.setDistance(SketchLine_2.endPoint(), SketchPoint_1.coordinates(), 20, True)
SketchConstraintDistance_2 = Sketch_1.setDistance(SketchLine_8.startPoint(), SketchPoint_1.coordinates(), 20, True)
SketchConstraintDistance_3 = Sketch_1.setDistance(SketchPoint_1.coordinates(), SketchLine_3.endPoint(), 40, True)
SketchConstraintEqual_5 = Sketch_1.setEqual(SketchLine_3.result(), SketchLine_8.result())
SketchLine_9 = Sketch_1.addLine(19.58460105006861, -42.37517030669127, -30.26368721382418, -8.981336187802668)
SketchConstraintCoincidence_9 = Sketch_1.setCoincident(SketchPoint_1.coordinates(), SketchLine_9.result())
SketchConstraintEqual_6 = Sketch_1.setEqual(SketchLine_3.result(), SketchLine_9.result())
SketchConstraintDistance_4 = Sketch_1.setDistance(SketchLine_9.startPoint(), SketchPoint_1.coordinates(), 20, True)
SketchConstraintAngle_7 = Sketch_1.setAngle(SketchLine_3.result(), SketchLine_8.result(), 20)
SketchConstraintAngle_8 = Sketch_1.setAngle(SketchLine_9.result(), SketchLine_8.result(), 340)
SketchLine_10 = Sketch_1.addLine(3.875430505227681, -29.75334619738324, -48.69363438803513, -0.8311502875301234)
SketchPoint_2 = Sketch_1.addPoint(-13.64759112585992, -20.1126142274322)
SketchConstraintCoincidence_10 = Sketch_1.setCoincident(SketchPoint_2.coordinates(), SketchLine_10.result())
SketchConstraintCoincidence_11 = Sketch_1.setCoincident(SketchPoint_2.coordinates(), SketchLine_9.result())
SketchConstraintDistance_5 = Sketch_1.setDistance(SketchPoint_2.coordinates(), SketchLine_10.startPoint(), 20, True)
SketchConstraintDistance_6 = Sketch_1.setDistance(SketchPoint_2.coordinates(), SketchLine_9.endPoint(), 20, True)
SketchConstraintEqual_7 = Sketch_1.setEqual(SketchLine_9.result(), SketchLine_10.result())
SketchLine_11 = Sketch_1.addLine(0.001529676402264532, -26.13785899518266, -31.94417752476854, -12.03580011868412)
SketchConstraintCoincidence_12 = Sketch_1.setCoincident(SketchPoint_2.coordinates(), SketchLine_11.result())
SketchConstraintDistance_7 = Sketch_1.setDistance(SketchLine_11.endPoint(), SketchPoint_2.coordinates(), 20, True)
SketchConstraintAngle_9 = Sketch_1.setAngle(SketchLine_9.result(), SketchLine_10.result(), 185)
SketchConstraintAngle_10 = Sketch_1.setAngle(SketchLine_11.result(), SketchLine_10.result(), 355)
SketchLine_12 = Sketch_1.addLine(5.283311970593094, -26.56404104269606, -51.50939731876596, -7.20976059690448)
SketchConstraintCoincidence_13 = Sketch_1.setCoincident(SketchPoint_2.coordinates(), SketchLine_12.result())
SketchConstraintEqual_8 = Sketch_1.setEqual(SketchLine_9.result(), SketchLine_12.result())
SketchConstraintDistance_8 = Sketch_1.setDistance(SketchPoint_2.coordinates(), SketchLine_12.startPoint(), 20, True)
SketchConstraintAngle_11 = Sketch_1.setAngle(SketchLine_12.result(), SketchLine_11.result(), 355)
SketchLine_13 = Sketch_1.addLine(25.19469725542325, -29.66649476390714, -33.06873531650151, -15.33567395919474)
SketchConstraintCoincidence_14 = Sketch_1.setCoincident(SketchPoint_2.coordinates(), SketchLine_13.result())
SketchConstraintEqual_9 = Sketch_1.setEqual(SketchLine_12.result(), SketchLine_13.result())
SketchConstraintDistance_9 = Sketch_1.setDistance(SketchLine_13.endPoint(), SketchPoint_2.coordinates(), 20, True)
SketchConstraintAngle_12 = Sketch_1.setAngle(SketchLine_12.result(), SketchLine_13.result(), 185)
SketchLine_14 = Sketch_1.addLine(25.87956617560386, -26.24481087018098, -33.41116977659181, -17.04651590605781)
SketchConstraintCoincidence_15 = Sketch_1.setCoincident(SketchPoint_2.coordinates(), SketchLine_14.result())
SketchConstraintDistance_10 = Sketch_1.setDistance(SketchLine_14.endPoint(), SketchPoint_2.coordinates(), 20, True)
SketchConstraintAngle_13 = Sketch_1.setAngle(SketchLine_13.result(), SketchLine_14.result(), 5)
SketchConstraintEqual_10 = Sketch_1.setEqual(SketchLine_13.result(), SketchLine_14.result())
SketchLine_15 = Sketch_1.addLine(26.26360956157018, -22.77645725728195, -33.60319146957497, -18.78069271250733)
SketchConstraintCoincidence_16 = Sketch_1.setCoincident(SketchPoint_2.coordinates(), SketchLine_15.result())
SketchConstraintEqual_11 = Sketch_1.setEqual(SketchLine_14.result(), SketchLine_15.result())
SketchConstraintDistance_11 = Sketch_1.setDistance(SketchLine_15.endPoint(), SketchPoint_2.coordinates(), 20, True)
SketchConstraintAngle_14 = Sketch_1.setAngle(SketchLine_15.result(), SketchLine_14.result(), 355)
SketchConstraintCoincidence_17 = Sketch_1.setCoincident(SketchLine_7.startPoint(), SketchLine_15.result())

SketchLine_16 = Sketch_1.addLine(60.00388909712782, -76.09960829009104, 78.00643494689578, -67.38706888260469)
SketchLine_17 = Sketch_1.addLine(78.00643494689578, -67.38706888260469, 86.19687277706721, -49.141063069)
SketchConstraintCoincidence_18 = Sketch_1.setCoincident(SketchLine_16.endPoint(), SketchLine_17.startPoint())
SketchLine_18 = Sketch_1.addLine(86.19687277706721, -49.141063069, 69.83467154684908, 8.584826884311665)
SketchConstraintCoincidence_19 = Sketch_1.setCoincident(SketchLine_17.endPoint(), SketchLine_18.startPoint())
SketchLine_19 = Sketch_1.addLine(60.00388909712782, -76.09960829009104, 40.61282650688053, -71.20198727360641)
SketchConstraintCoincidence_20 = Sketch_1.setCoincident(SketchLine_16.startPoint(), SketchLine_19.startPoint())
SketchLine_20 = Sketch_1.addLine(28.90654086978602, -54.98585713775849, 40.61282650688053, -71.20198727360641)
SketchConstraintCoincidence_21 = Sketch_1.setCoincident(SketchLine_19.endPoint(), SketchLine_20.endPoint())
SketchLine_21 = Sketch_1.addLine(28.90654086978602, -54.98585713775849, 30.36253333631381, -35.03892539532215)
SketchConstraintCoincidence_22 = Sketch_1.setCoincident(SketchLine_20.startPoint(), SketchLine_21.startPoint())
SketchLine_22 = Sketch_1.addLine(47.45878924889096, -17.44298467461911, 30.36253333631381, -35.03892539532215)
SketchConstraintCoincidence_23 = Sketch_1.setCoincident(SketchLine_21.endPoint(), SketchLine_22.endPoint())
SketchConstraintAngle_15 = Sketch_1.setAngleComplementary(SketchLine_16.result(), SketchLine_17.result(), 40)
SketchConstraintAngle_16 = Sketch_1.setAngleComplementary(SketchLine_18.result(), SketchLine_17.result(), 40)
SketchConstraintAngle_17 = Sketch_1.setAngleComplementary(SketchLine_16.result(), SketchLine_19.result(), 40)
SketchConstraintAngle_18 = Sketch_1.setAngleComplementary(SketchLine_21.result(), SketchLine_20.result(), 40)
SketchConstraintAngle_19 = Sketch_1.setAngleComplementary(SketchLine_20.result(), SketchLine_19.result(), 40)
SketchConstraintAngle_20 = Sketch_1.setAngleComplementary(SketchLine_21.result(), SketchLine_22.result(), 40)
SketchConstraintLength_2 = Sketch_1.setLength(SketchLine_17.result(), 20)
SketchConstraintEqual_12 = Sketch_1.setEqual(SketchLine_17.result(), SketchLine_16.result())
SketchConstraintEqual_13 = Sketch_1.setEqual(SketchLine_17.result(), SketchLine_19.result())
SketchConstraintEqual_14 = Sketch_1.setEqual(SketchLine_17.result(), SketchLine_20.result())
SketchConstraintEqual_15 = Sketch_1.setEqual(SketchLine_17.result(), SketchLine_21.result())
SketchLine_23 = Sketch_1.addLine(92.44909133742235, -46.11522988707736, 57.3302344261388, 2.533160520466375)
SketchPoint_3 = Sketch_1.addPoint(80.74280570032784, -29.89909975122945)
SketchConstraintCoincidence_24 = Sketch_1.setCoincident(SketchPoint_3.coordinates(), SketchLine_18.result())
SketchConstraintCoincidence_25 = Sketch_1.setCoincident(SketchPoint_3.coordinates(), SketchLine_23.result())
SketchConstraintDistance_12 = Sketch_1.setDistance(SketchLine_17.endPoint(), SketchPoint_3.coordinates(), 20, True)
SketchConstraintDistance_13 = Sketch_1.setDistance(SketchLine_23.startPoint(), SketchPoint_3.coordinates(), 20, True)
SketchConstraintDistance_14 = Sketch_1.setDistance(SketchPoint_3.coordinates(), SketchLine_18.endPoint(), 40, True)
SketchConstraintEqual_16 = Sketch_1.setEqual(SketchLine_18.result(), SketchLine_23.result())
SketchLine_24 = Sketch_1.addLine(97.289359083568, -41.13349208617944, 47.64969893384747, -7.430315081329468)
SketchConstraintCoincidence_26 = Sketch_1.setCoincident(SketchPoint_3.coordinates(), SketchLine_24.result())
SketchConstraintEqual_17 = Sketch_1.setEqual(SketchLine_18.result(), SketchLine_24.result())
SketchConstraintDistance_15 = Sketch_1.setDistance(SketchLine_24.startPoint(), SketchPoint_3.coordinates(), 20, True)
SketchConstraintAngle_21 = Sketch_1.setAngleComplementary(SketchLine_18.result(), SketchLine_23.result(), 160)
SketchConstraintAngle_22 = Sketch_1.setAngleComplementary(SketchLine_24.result(), SketchLine_23.result(), 160)
SketchLine_25 = Sketch_1.addLine(81.65898287743471, -28.41422234660911, 29.27079119639352, 0.8343224443798635)
SketchPoint_4 = Sketch_1.addPoint(64.19625231708766, -18.66470741627946)
SketchConstraintCoincidence_27 = Sketch_1.setCoincident(SketchPoint_4.coordinates(), SketchLine_25.result())
SketchConstraintCoincidence_28 = Sketch_1.setCoincident(SketchPoint_4.coordinates(), SketchLine_24.result())
SketchConstraintDistance_16 = Sketch_1.setDistance(SketchPoint_4.coordinates(), SketchLine_25.startPoint(), 20, True)
SketchConstraintDistance_17 = Sketch_1.setDistance(SketchPoint_4.coordinates(), SketchLine_24.endPoint(), 20, True)
SketchConstraintEqual_18 = Sketch_1.setEqual(SketchLine_24.result(), SketchLine_25.result())
SketchLine_26 = Sketch_1.addLine(95.51651619313817, -32.72404055044315, 45.95024650348294, -10.47426958610802)
SketchConstraintCoincidence_29 = Sketch_1.setCoincident(SketchPoint_4.coordinates(), SketchLine_26.result())
SketchConstraintDistance_18 = Sketch_1.setDistance(SketchLine_26.endPoint(), SketchPoint_4.coordinates(), 20, True)
SketchConstraintAngle_23 = Sketch_1.setAngleComplementary(SketchLine_24.result(), SketchLine_25.result(), 5)
SketchConstraintAngle_24 = Sketch_1.setAngleComplementary(SketchLine_26.result(), SketchLine_25.result(), 5)
SketchLine_27 = Sketch_1.addLine(83.08667026246893, -25.23373396888348, 26.41541642632507, -5.526654311071399)
SketchConstraintCoincidence_30 = Sketch_1.setCoincident(SketchPoint_4.coordinates(), SketchLine_27.result())
SketchConstraintEqual_19 = Sketch_1.setEqual(SketchLine_24.result(), SketchLine_27.result())
SketchConstraintDistance_19 = Sketch_1.setDistance(SketchPoint_4.coordinates(), SketchLine_27.startPoint(), 20, True)
SketchConstraintAngle_25 = Sketch_1.setAngleComplementary(SketchLine_27.result(), SketchLine_26.result(), 5)
SketchLine_28 = Sketch_1.addLine(102.9783774975822, -28.45994944924871, 44.80518972684035, -13.76708639979482)
SketchConstraintCoincidence_31 = Sketch_1.setCoincident(SketchPoint_4.coordinates(), SketchLine_28.result())
SketchConstraintEqual_20 = Sketch_1.setEqual(SketchLine_27.result(), SketchLine_28.result())
SketchConstraintDistance_20 = Sketch_1.setDistance(SketchLine_28.endPoint(), SketchPoint_4.coordinates(), 20, True)
SketchConstraintAngle_26 = Sketch_1.setAngleComplementary(SketchLine_27.result(), SketchLine_28.result(), 5)
SketchLine_29 = Sketch_1.addLine(103.6845113974033, -25.04259067061018, 44.45212277692985, -15.47576578911409)
SketchConstraintCoincidence_32 = Sketch_1.setCoincident(SketchPoint_4.coordinates(), SketchLine_29.result())
SketchConstraintDistance_21 = Sketch_1.setDistance(SketchLine_29.endPoint(), SketchPoint_4.coordinates(), 20, True)
SketchConstraintAngle_27 = Sketch_1.setAngleComplementary(SketchLine_28.result(), SketchLine_29.result(), 175)
SketchConstraintEqual_21 = Sketch_1.setEqual(SketchLine_28.result(), SketchLine_29.result())
SketchLine_30 = Sketch_1.addLine(104.0901158019603, -21.57669234933498, 44.24932057465131, -17.20871494975169)
SketchConstraintCoincidence_33 = Sketch_1.setCoincident(SketchPoint_4.coordinates(), SketchLine_30.result())
SketchConstraintEqual_22 = Sketch_1.setEqual(SketchLine_29.result(), SketchLine_30.result())
SketchConstraintDistance_22 = Sketch_1.setDistance(SketchLine_30.endPoint(), SketchPoint_4.coordinates(), 20, True)
SketchConstraintAngle_28 = Sketch_1.setAngleComplementary(SketchLine_30.result(), SketchLine_29.result(), 175)
SketchConstraintCoincidence_34 = Sketch_1.setCoincident(SketchLine_22.startPoint(), SketchLine_30.result())

SketchLine_31 = Sketch_1.addLine(138.7804692087479, -78.2421341225341, 156.7284871354776, -69.4178124771163)
SketchLine_32 = Sketch_1.addLine(156.7284871354776, -69.4178124771163, 164.8053019156854, -51.12122637461573)
SketchConstraintCoincidence_35 = Sketch_1.setCoincident(SketchLine_31.endPoint(), SketchLine_32.startPoint())
SketchLine_33 = Sketch_1.addLine(164.8053019156854, -51.12122637461573, 148.0844426183728, 6.501797360341752)
SketchConstraintCoincidence_36 = Sketch_1.setCoincident(SketchLine_32.endPoint(), SketchLine_33.startPoint())
SketchLine_34 = Sketch_1.addLine(138.7804692087479, -78.2421341225341, 119.3593251934135, -73.46519314156717)
SketchConstraintCoincidence_37 = Sketch_1.setCoincident(SketchLine_31.startPoint(), SketchLine_34.startPoint())
SketchLine_35 = Sketch_1.addLine(107.5524242162226, -57.32217330899532, 119.3593251934135, -73.46519314156717)
SketchConstraintCoincidence_38 = Sketch_1.setCoincident(SketchLine_34.endPoint(), SketchLine_35.endPoint())
SketchLine_36 = Sketch_1.addLine(107.5524242162226, -57.32217330899532, 108.8843464634908, -37.36657301415998)
SketchConstraintCoincidence_39 = Sketch_1.setCoincident(SketchLine_35.startPoint(), SketchLine_36.startPoint())
SketchLine_37 = Sketch_1.addLine(125.8708496231354, -19.66465768043951, 108.8843464634908, -37.36657301415998)
SketchConstraintCoincidence_40 = Sketch_1.setCoincident(SketchLine_36.endPoint(), SketchLine_37.endPoint())
SketchConstraintAngle_29 = Sketch_1.setAngleBackward(SketchLine_31.result(), SketchLine_32.result(), 140)
SketchConstraintAngle_30 = Sketch_1.setAngleBackward(SketchLine_33.result(), SketchLine_32.result(), 220)
SketchConstraintAngle_31 = Sketch_1.setAngleBackward(SketchLine_31.result(), SketchLine_34.result(), 220)
SketchConstraintAngle_32 = Sketch_1.setAngleBackward(SketchLine_36.result(), SketchLine_35.result(), 140)
SketchConstraintAngle_33 = Sketch_1.setAngleBackward(SketchLine_35.result(), SketchLine_34.result(), 140)
SketchConstraintAngle_34 = Sketch_1.setAngleBackward(SketchLine_36.result(), SketchLine_37.result(), 220)
SketchConstraintLength_3 = Sketch_1.setLength(SketchLine_32.result(), 20)
SketchConstraintEqual_23 = Sketch_1.setEqual(SketchLine_32.result(), SketchLine_31.result())
SketchConstraintEqual_24 = Sketch_1.setEqual(SketchLine_32.result(), SketchLine_34.result())
SketchConstraintEqual_25 = Sketch_1.setEqual(SketchLine_32.result(), SketchLine_35.result())
SketchConstraintEqual_26 = Sketch_1.setEqual(SketchLine_32.result(), SketchLine_36.result())
SketchLine_38 = Sketch_1.addLine(171.0385831271055, -48.05657162886843, 135.6178801955327, 0.3724878688471688)
SketchPoint_5 = Sketch_1.addPoint(159.2316821499145, -31.91355179629657)
SketchConstraintCoincidence_41 = Sketch_1.setCoincident(SketchPoint_5.coordinates(), SketchLine_33.result())
SketchConstraintCoincidence_42 = Sketch_1.setCoincident(SketchPoint_5.coordinates(), SketchLine_38.result())
SketchConstraintDistance_23 = Sketch_1.setDistance(SketchLine_32.endPoint(), SketchPoint_5.coordinates(), 20, True)
SketchConstraintDistance_24 = Sketch_1.setDistance(SketchLine_38.startPoint(), SketchPoint_5.coordinates(), 20, True)
SketchConstraintDistance_25 = Sketch_1.setDistance(SketchPoint_5.coordinates(), SketchLine_33.endPoint(), 40, True)
SketchConstraintEqual_27 = Sketch_1.setEqual(SketchLine_33.result(), SketchLine_38.result())
SketchLine_39 = Sketch_1.addLine(175.8477778293761, -43.04483044571408, 125.9994907909914, -9.650994497461493)
SketchConstraintCoincidence_43 = Sketch_1.setCoincident(SketchPoint_5.coordinates(), SketchLine_39.result())
SketchConstraintEqual_28 = Sketch_1.setEqual(SketchLine_33.result(), SketchLine_39.result())
SketchConstraintDistance_26 = Sketch_1.setDistance(SketchLine_39.startPoint(), SketchPoint_5.coordinates(), 20, True)
SketchConstraintAngle_35 = Sketch_1.setAngleBackward(SketchLine_33.result(), SketchLine_38.result(), 340)
SketchConstraintAngle_36 = Sketch_1.setAngleBackward(SketchLine_39.result(), SketchLine_38.result(), 20)
SketchLine_40 = Sketch_1.addLine(160.1386077477389, -30.42300575990098, 107.5695439158812, -1.500807920835093)
SketchPoint_6 = Sketch_1.addPoint(142.615586470453, -20.78227314687903)
SketchConstraintCoincidence_44 = Sketch_1.setCoincident(SketchPoint_6.coordinates(), SketchLine_40.result())
SketchConstraintCoincidence_45 = Sketch_1.setCoincident(SketchPoint_6.coordinates(), SketchLine_39.result())
SketchConstraintDistance_27 = Sketch_1.setDistance(SketchPoint_6.coordinates(), SketchLine_40.startPoint(), 20, True)
SketchConstraintDistance_28 = Sketch_1.setDistance(SketchPoint_6.coordinates(), SketchLine_39.endPoint(), 20, True)
SketchConstraintEqual_29 = Sketch_1.setEqual(SketchLine_39.result(), SketchLine_40.result())
SketchLine_41 = Sketch_1.addLine(147.1612701207084, -22.78891223719074, 124.3190003679524, -12.70545836667124)
SketchConstraintCoincidence_46 = Sketch_1.setCoincident(SketchPoint_6.coordinates(), SketchLine_41.result())
SketchConstraintDistance_29 = Sketch_1.setDistance(SketchLine_41.endPoint(), SketchPoint_6.coordinates(), 20, True)
SketchConstraintAngle_37 = Sketch_1.setAngleBackward(SketchLine_39.result(), SketchLine_40.result(), 175)
SketchConstraintAngle_38 = Sketch_1.setAngleBackward(SketchLine_41.result(), SketchLine_40.result(), 5)
SketchLine_42 = Sketch_1.addLine(161.5464893301474, -27.23370065688114, 104.7537807510641, -7.879418126874779)
SketchConstraintCoincidence_47 = Sketch_1.setCoincident(SketchPoint_6.coordinates(), SketchLine_42.result())
SketchConstraintEqual_30 = Sketch_1.setEqual(SketchLine_39.result(), SketchLine_42.result())
SketchConstraintDistance_30 = Sketch_1.setDistance(SketchPoint_6.coordinates(), SketchLine_42.startPoint(), 20, True)
SketchConstraintAngle_39 = Sketch_1.setAngleBackward(SketchLine_42.result(), SketchLine_41.result(), 5)
SketchLine_43 = Sketch_1.addLine(181.4578745011218, -30.33615510881286, 123.1944424551186, -16.0053321659121)
SketchConstraintCoincidence_48 = Sketch_1.setCoincident(SketchPoint_6.coordinates(), SketchLine_43.result())
SketchConstraintEqual_31 = Sketch_1.setEqual(SketchLine_42.result(), SketchLine_43.result())
SketchConstraintDistance_31 = Sketch_1.setDistance(SketchLine_43.endPoint(), SketchPoint_6.coordinates(), 20, True)
SketchConstraintAngle_40 = Sketch_1.setAngleBackward(SketchLine_42.result(), SketchLine_43.result(), 175)
SketchLine_44 = Sketch_1.addLine(182.1427435468735, -26.91447124022047, 122.8520079322427, -17.71617410020831)
SketchConstraintCoincidence_49 = Sketch_1.setCoincident(SketchPoint_6.coordinates(), SketchLine_44.result())
SketchConstraintDistance_32 = Sketch_1.setDistance(SketchLine_44.endPoint(), SketchPoint_6.coordinates(), 20, True)
SketchConstraintAngle_41 = Sketch_1.setAngleBackward(SketchLine_43.result(), SketchLine_44.result(), 355)
SketchConstraintEqual_32 = Sketch_1.setEqual(SketchLine_43.result(), SketchLine_44.result())
SketchLine_45 = Sketch_1.addLine(182.5267870601237, -23.44611764141531, 122.6599861756176, -19.45035089961089)
SketchConstraintCoincidence_50 = Sketch_1.setCoincident(SketchPoint_6.coordinates(), SketchLine_45.result())
SketchConstraintEqual_33 = Sketch_1.setEqual(SketchLine_44.result(), SketchLine_45.result())
SketchConstraintDistance_33 = Sketch_1.setDistance(SketchLine_45.endPoint(), SketchPoint_6.coordinates(), 20, True)
SketchConstraintAngle_42 = Sketch_1.setAngleBackward(SketchLine_45.result(), SketchLine_44.result(), 5)
SketchConstraintCoincidence_51 = Sketch_1.setCoincident(SketchLine_37.startPoint(), SketchLine_45.result())
model.do()

Extrusion_1_objects = [model.selection("FACE", "Sketch_1/Face-SketchLine_1r-SketchLine_2f-SketchLine_3f-SketchLine_9f-SketchLine_15f-SketchLine_7f-SketchLine_6r-SketchLine_5f-SketchLine_4r"), model.selection("FACE", "Sketch_1/Face-SketchLine_16f-SketchLine_17f-SketchLine_18f-SketchLine_24f-SketchLine_30f-SketchLine_22f-SketchLine_21r-SketchLine_20f-SketchLine_19r"), model.selection("FACE", "Sketch_1/Face-SketchLine_31f-SketchLine_32f-SketchLine_33f-SketchLine_39f-SketchLine_45f-SketchLine_37f-SketchLine_36r-SketchLine_35f-SketchLine_34r")]
Extrusion_1 = model.addExtrusion(Part_1_doc, Extrusion_1_objects, model.selection(), 10, 0)

model.end()

from GeomAPI import *

REF_VOLUME = 25018.7130187615

model.testNbResults(Extrusion_1, 3)
model.testNbSubResults(Extrusion_1, [0, 0, 0])
model.testNbSubShapes(Extrusion_1, GeomAPI_Shape.SOLID, [1, 1, 1])
model.testNbSubShapes(Extrusion_1, GeomAPI_Shape.FACE, [11, 11, 11])
model.testNbSubShapes(Extrusion_1, GeomAPI_Shape.EDGE, [54, 54, 54])
model.testNbSubShapes(Extrusion_1, GeomAPI_Shape.VERTEX, [108, 108, 108])
model.testResultsVolumes(Extrusion_1, [REF_VOLUME, REF_VOLUME, REF_VOLUME])

assert(model.checkPythonDump())
