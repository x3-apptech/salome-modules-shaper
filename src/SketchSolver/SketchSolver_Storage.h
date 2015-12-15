// Copyright (C) 2014-20xx CEA/DEN, EDF R&D

// File:    SketchSolver_Storage.h
// Created: 18 Mar 2015
// Author:  Artem ZHIDKOV

#ifndef SketchSolver_Storage_H_
#define SketchSolver_Storage_H_

#include "SketchSolver.h"
#include <SketchSolver_Solver.h>

#include <list>
#include <memory>
#include <set>
#include <vector>

/** \class   SketchSolver_Storage
 *  \ingroup Plugins
 *  \brief   Contains all necessary data in SolveSpace format to solve a single group of constraints
 */
class SketchSolver_Storage
{
public:
  SketchSolver_Storage();

  /** \brief Add new parameter to the current group
   *  \param[in] theParam  SolveSpace parameter
   *  \return the ID of added parameter
   */
  Slvs_hParam addParameter(const Slvs_Param& theParam);
  /** \brief Updates parameter in the current group. If the ID of parameter is zero, the new item will be added
   *  \param[in] theParam  SolveSpace parameter
   *  \return the ID of updated/added parameter
   */
  Slvs_hParam updateParameter(const Slvs_Param& theParam);
  /** \brief Removes the parameter by its ID
   *  \param[in] theParamID  index of parameter to be removed
   *  \return \c true if the parameter was successfully removed
   */
  bool removeParameter(const Slvs_hParam& theParamID);
  /// \brief Returns the parameter by its ID
  const Slvs_Param& getParameter(const Slvs_hParam& theParamID) const;

  /** \brief Add new entity to the current group
   *  \param[in] theEntity  SolveSpace entity
   *  \return the ID of added entity
   */
  Slvs_hEntity addEntity(const Slvs_Entity& theEntity);
  /** \brief Updates entity in the current group. If the ID of entity is zero, the new item will be added
   *  \param[in] theEntity  SolveSpace entity
   *  \return the ID of updated/added entity
   */
  Slvs_hEntity updateEntity(const Slvs_Entity& theEntity);
  /** \brief Removes the entity by its ID. All parameters used in this entity,
   *         and not used in other constraints, will be removed too.
   *  \param[in] theEntityID  index of entity to be removed
   *  \return \c true if the entity was successfully removed
   */
  bool removeEntity(const Slvs_hEntity& theEntityID);
  /** \brief Remove all entities, which are not used in constraints
   */
  void removeUnusedEntities();
  /// \brief Returns the entity by its ID
  const Slvs_Entity& getEntity(const Slvs_hEntity& theEntityID) const;
  /// \brief Makes a full copy of the given entity
  Slvs_hEntity copyEntity(const Slvs_hEntity& theCopied);
  /// \brief Copy one entity to another
  void copyEntity(const Slvs_hEntity& theFrom, const Slvs_hEntity& theTo);
  /// \brief Check the entity is used in constraints
  bool isUsedByConstraints(const Slvs_hEntity& theEntityID) const;
  /// \brief Returns maximal ID of entities in this storage
  const Slvs_hEntity& entityMaxID() const
  { return myEntityMaxID; }

  /// \brief Verifies the current point or another coincident one is fixed
  /// \param[in]  thePointID  entity to be checked fixed
  /// \param[out] theFixed    ID of constraint
  /// \param[in]  theAccurate if \c true, the calculation will be made for all type of constraints,
  ///                         if \c false, only the point is verified
  /// \return \c true if the point is fixed
  bool isPointFixed(const Slvs_hEntity& thePointID, Slvs_hConstraint& theFixed, bool theAccurate = false) const;
  /// \brief Verifies the current entity is fully fixed (may not be changed by constraints)
  /// \param[in] theEntityID entity to be checked fixed
  /// \param[in] theAccurate if \c true, the calculation will be made for all type of constraints,
  ///                        if \c false, only points are verified
  /// \return \c true if the entity is fixed
  bool isEntityFixed(const Slvs_hEntity& theEntityID, bool theAccurate = false) const;

  /** \brief Add new constraint to the current group
   *  \param[in] theConstraint   SolveSpace's constraint
   *  \return the ID of added constraint
   */
  Slvs_hConstraint addConstraint(const Slvs_Constraint& theConstraint);
  /** \brief Updates constraint in the current group.
   *         If the ID of constraint is zero, the new item will be added
   *  \param[in] theConstraint  SolveSpace constraint
   *  \return the ID of updated/added constraint
   */
  Slvs_hConstraint updateConstraint(const Slvs_Constraint& theConstraint);
  /** \brief Removes the constraint by its ID. All entities and parameters depending on this
   *         constraint, which are not used in other constraints, will be removed too.
   *  \param[in] theConstraintID  index of constraint to be removed
   *  \return \c true if the constraint was successfully removed
   */
  bool removeConstraint(const Slvs_hConstraint& theConstraintID);
  /// \brief Returns the constraint by its ID
  const Slvs_Constraint& getConstraint(const Slvs_hConstraint& theConstraintID) const;
  /// \brief Returns list of constraints of specified type
  std::list<Slvs_Constraint> getConstraintsByType(int theConstraintType) const;
  /// \brief Returns quantity of constraints in this storage
  size_t nbConstraints() const
  { return myConstraints.size(); }

  /// \brief Attach constraint SLVS_C_WHERE_DRAGGED to this storage. It need to make precise calculations
  void addConstraintWhereDragged(const Slvs_hConstraint& theConstraintID);

  /// \brief Add transient constraint
  void addTemporaryConstraint(const Slvs_hConstraint& theConstraintID);
  /// \brief Remove all transient constraints
  void removeTemporaryConstraints();
  /// \brief Remove one temporary constraint. Preferable to remove the points under Point-on-Line constraint
  /// \return Number of remaining temporary constraints
  int deleteTemporaryConstraint();
  /// \brief Checks the constraint is temporary
  bool isTemporary(const Slvs_hConstraint& theConstraintID) const;
  /// \brief Number of temporary constraints
  int numberTemporary() const
  { return (int)myTemporaryConstraints.size(); }

  /// \brief Shows the sketch should be resolved
  bool isNeedToResolve();

  /// \brief Shows the storage has the same constraint twice
  bool hasDuplicatedConstraint() const
  { return myDuplicatedConstraint; }

  /// \brief Changes the flag of group to be resolved
  void setNeedToResolve(bool theFlag)
  { myNeedToResolve = theFlag; }

  /// \brief Returns lists of removed elements
  void getRemoved(std::set<Slvs_hParam>& theParameters,
                  std::set<Slvs_hEntity>& theEntities,
                  std::set<Slvs_hConstraint>& theConstraints);

  /// \brief Initialize constraint solver by the entities collected by current storage
  void initializeSolver(SketchSolver_Solver& theSolver);

private:
  /// \brief Store coincident points
  void addCoincidentPoints(const Slvs_hEntity& thePoint1, const Slvs_hEntity& thePoint2);
  /// \brief Remove point from lists of coincidence
  void removeCoincidentPoint(const Slvs_hEntity& thePoint);
  /// \brief Remove point-point coincidence
  void removeCoincidence(const Slvs_Constraint& theCoincidence);

public:
  /// \brief Check two points are coincident
  bool isCoincident(const Slvs_hEntity& thePoint1, const Slvs_hEntity& thePoint2) const;

  /// \brief Check two points are coincident or have same coordinates
  bool isEqual(const Slvs_hEntity& thePoint1, const Slvs_hEntity& thePoint2) const;

  /// \brief Check the entity is horizontal of vertical
  bool isAxisParallel(const Slvs_hEntity& theEntity) const;

  /// \brief Verifies the entity is used in any equal constraint
  /// \param[in]  theEntity entity to be found
  /// \param[out] theEqual  constraint, which uses the entity
  /// \return \c true, if the Equal constrait is found
  bool isUsedInEqual(const Slvs_hEntity& theEntity, Slvs_Constraint& theEqual) const;

  /// \brief Fixes specified entity
  /// \param theEntity  ID of the entity to be fixed
  /// \return List of created constraints
  std::vector<Slvs_hConstraint> fixEntity(const Slvs_hEntity& theEntity);

private:
  /// \brief Fixes specified point
  /// \param [in]  thePoint    point to be fixed
  /// \param [out] theCreated  list of the Fixed constraints created
  void fixPoint(const Slvs_Entity& thePoint, std::vector<Slvs_hConstraint>& theCreated);
  /// \brief Fixes specified line
  /// \param [in]  theLine     line to be fixed
  /// \param [out] theCreated  list of the Fixed constraints created
  void fixLine(const Slvs_Entity& theLine, std::vector<Slvs_hConstraint>& theCreated);
  /// \brief Fixes specified circle
  /// \param [in]  theCircle   circle to be fixed
  /// \param [out] theCreated  list of the Fixed constraints created
  void fixCircle(const Slvs_Entity& theCircle, std::vector<Slvs_hConstraint>& theCreated);
  /// \brief Fixes specified arc
  /// \param [in]  theArc      arc to be fixed
  /// \param [out] theCreated  list of the Fixed constraints created
  void fixArc(const Slvs_Entity& theArc, std::vector<Slvs_hConstraint>& theCreated);

private:
  Slvs_hParam myParamMaxID; ///< current parameter index (may differs with the number of parameters)
  std::vector<Slvs_Param> myParameters; ///< list of parameters used in the current group of constraints (sorted by the identifier)
  Slvs_hEntity myEntityMaxID; ///< current entity index (may differs with the number of entities)
  std::vector<Slvs_Entity> myEntities; ///< list of entities used in the current group of constraints (sorted by the identifier)
  Slvs_hConstraint myConstrMaxID; ///< current constraint index (may differs with the number of constraints)
  std::vector<Slvs_Constraint> myConstraints; ///< list of constraints used in the current group (sorted by the identifier)

  std::vector< std::set<Slvs_hEntity> > myCoincidentPoints; ///< lists of coincident points
  Slvs_hConstraint myFixed; ///< identifier of one of temporary constraints to fix separate point

  bool myNeedToResolve; ///< parameters are changed and group needs to be resolved
  bool myDuplicatedConstraint; ///< shows the storage has same constraint twice

  std::set<Slvs_hConstraint> myTemporaryConstraints; ///< list of transient constraints
  std::set<Slvs_hParam> myRemovedParameters; ///< list of just removed parameters (cleared when returning to applicant)
  std::set<Slvs_hEntity> myRemovedEntities; ///< list of just removed entities (cleared when returning to applicant)
  std::set<Slvs_hConstraint> myRemovedConstraints; ///< list of just removed constraints (cleared when returning to applicant)
  std::set<Slvs_hParam> myUpdatedParameters; ///< list of just updated parameters (cleared when isNeedToResolve() called)
};

typedef std::shared_ptr<SketchSolver_Storage> StoragePtr;

#endif
