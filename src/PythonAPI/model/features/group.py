"""Group Interface
Author: Sergey Pokhodenko
Copyright (C) 2014-20xx CEA/DEN, EDF R&D
"""

from model.roots import Interface


def addGroup(part, *args):
    """Add an Group feature to the Part and return Group.

    Pass all args to Group __init__ function.
    """
    assert(args)
    feature = part.addFeature("Group")
    return Group(feature, *args)


class Group(Interface):
    """Interface class for Group feature.

    Group(feature) -> feature interface without initialization
    Group(feature, group_list) ->
        feature interface initialized from arguments:
        - group_list
    """

    def __init__(self, feature, *args):
        """x.__init__(...) initializes x; see x.__class__.__doc__ for signature"""
        Interface.__init__(self, feature)
        assert(self._feature.getKind() == "Group")

        self._group_list = self._feature.data().selectionList("group_list")

        assert(self._group_list)

        if not args:
            return

        assert(len(args) == 1)
        self.setGroupList(args[0])

        self.execute()
        pass

    def setGroupList(self, main_objects):
        """Modify group_list attribute of the feature.

        See __init__.
        """
        self._fillAttribute(self._group_list, group_list)
        pass
