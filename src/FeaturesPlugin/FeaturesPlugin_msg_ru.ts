<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE TS>
<TS version="2.0" language="ru_RU">
  <context>
    <name>Boolean</name>
    <message>
      <source>Boolean</source>
      <translation>Булевы операции</translation>
    </message>
    <message>
      <source>Perform boolean operations with objects</source>
      <translation>Производит булевы операции с объектами</translation>
    </message>
  </context>
  <context>
    <name>Boolean:bool_type</name>
    <message>
      <source>Operation type</source>
      <translation>Тип операции</translation>
    </message>
    <message>
      <source>Type of boolean operation</source>
      <translation>Тип булевой операции</translation>
    </message>
    <message>
      <source>Cut</source>
      <translation>Вырезание</translation>
    </message>
    <message>
      <source>Fuse</source>
      <translation>Объединение</translation>
    </message>
    <message>
      <source>Common</source>
      <translation>Пересечение</translation>
    </message>
    <message>
      <source>Fill</source>
      <translation>Заполнение</translation>
    </message>
    <message>
      <source>Smash</source>
      <translation>Вдавливание</translation>
    </message>
  </context>
  <context>
    <name>Boolean:main_objects</name>
    <message>
      <source>Main objects</source>
      <translation>Основные объекты</translation>
    </message>
    <message>
      <source>Select objects</source>
      <translation>Выберите основные объекты</translation>
    </message>
  </context>
  <context>
    <name>Boolean:main_objects:FeaturesPlugin_ValidatorBooleanSelection</name>
    <message>
      <source>Error: Empty attribute selection.</source>
      <translation>Выберите основные объекты.</translation>
    </message>
    <message>
      <source>Error: Empty selection context.</source>
      <translation>Выбранные основные объекты невалидны.</translation>
    </message>
    <message>
      <source>Error: Result construction not allowed for selection.</source>
      <translation>Нельзя селектировать конструкции.</translation>
    </message>
    <message>
      <source>Error: Empty shape.</source>
      <translation>Выбранные основные объекты невалидны.</translation>
    </message>
    <message>
      <source>Error: Local selection not allowed.</source>
      <translation>Локальная селекция запрещена.</translation>
    </message>
    <message>
      <source>Error: Selected shape has the wrong type.</source>
      <translation>Выбраныне основные объекты имеют недопустимый тип.</translation>
    </message>
  </context>
  <context>
    <name>Boolean:tool_objects</name>
    <message>
      <source>Tool objects</source>
      <translation>Вспомогательные объекты</translation>
    </message>
    <message>
      <source>Select tools</source>
      <translation>Выберите вспомогательные объекты</translation>
    </message>
  </context>
  <context>
    <name>Boolean:tool_objects:FeaturesPlugin_ValidatorBooleanSelection</name>
    <message>
      <source>Error: Empty attribute selection.</source>
      <translation>Выберите вспомогательные объекты.</translation>
    </message>
    <message>
      <source>Error: Empty selection context.</source>
      <translation>Выбранные вспомогательные объекты невалидны.</translation>
    </message>
    <message>
      <source>Error: Result construction not allowed for selection.</source>
      <translation>Нельзя селектировать конструкции.</translation>
    </message>
    <message>
      <source>Error: Empty shape.</source>
      <translation>Выбранные вспомогательные объекты невалидны.</translation>
  </message>
    <message>
      <source>Error: Local selection not allowed.</source>
      <translation>Локальная селекция запрещена.</translation>
    </message>
    <message>
      <source>Error: Selected shape has the wrong type.</source>
      <translation>Выбраныне вспомогательные объекты имеют недопустимый тип.</translation>
    </message>
  </context>
  <context>
    <name>Boolean:GeomValidators_BooleanArguments</name>
    <message>
      <source>Not enough arguments</source>
      <translation>Недостаточно объектов для операции.</translation>
    </message>
  </context>
  <context>
    <name>Boolean:Model_FeatureValidator</name>
    <message>
      <source>Attribute "bool_type" is not initialized.</source>
      <translation>Выберите тип операции.</translation>
    </message>
    <message>
      <source>Attribute "main_objects" is not initialized.</source>
      <translation>Выберите основные объекты.</translation>
    </message>
    <message>
      <source>Attribute "tool_objects" is not initialized.</source>
      <translation>Выберите вспомогательные объекты.</translation>
    </message>
  </context>

  <!-- Defeaturing -->
  <context>
    <name>Defeaturing</name>
    <message>
      <source>Defeaturing</source>
      <translation>Удаление фичеров</translation>
    </message>
    <message>
      <source>Faces to remove</source>
      <translation>Удаляемые грани</translation>
    </message>
    <message>
      <source>Select faces</source>
      <translation>Выберите грани</translation>
    </message>
  </context>
  <context>
    <name>Defeaturing:FeaturesPlugin_ValidatorDefeaturingSelection</name>
    <message>
      <source>Error: This validator can only work with selection list attributes.</source>
      <translation>Ошибка: валидатор поддерживает только аттрибуты типа SelectionList.</translation>
    </message>
    <message>
      <source>Error: Empty attribute selection.</source>
      <translation>Ошибка: незаполненный аттрибут.</translation>
    </message>
    <message>
      <source>Error: Empty selection context.</source>
      <translation>Ошибка: пустой контекст.</translation>
    </message>
    <message>
      <source>Error: Not all selected shapes are sub-shapes of solids.</source>
      <translation>Ошибка: не все выбранные объекты являются подэлементами твердых тел.</translation>
    </message>
  </context>

  <!-- 1D-fillet -->
  <context>
    <name>Fillet1D</name>
    <message>
      <source>1D-fillet</source>
      <translation>1D-сглаживание</translation>
    </message>
    <message>
      <source>Perform fillet on vertices of a wire</source>
      <translation>Выполнить сглаживание в узлах контура</translation>
    </message>
    <message>
      <source>Wire has no vertices for fillet.</source>
      <translation>Контур не имеет узлов, подходящих для сглаживания.</translation>
    </message>
  </context>
  <context>
    <name>Fillet1D:main_wires</name>
    <message>
      <source>Wires</source>
      <translation>Контуры</translation>
    </message>
    <message>
      <source>Select wires</source>
      <translation>Выберите контуры</translation>
    </message>
    <message>
      <source>Attribute "%1" is not initialized.</source>
      <translation>Выберите контуры.</translation>
    </message>
    <message>
      <source>Fillet each sharp corner of the wire</source>
      <translation>Сгладить все острые углы контура</translation>
    </message>
  </context>
  <context>
    <name>Fillet1D:main_vertices</name>
    <message>
      <source>Vertices</source>
      <translation>Узлы</translation>
    </message>
    <message>
      <source>Select vertices</source>
      <translation>Выберите узлы</translation>
    </message>
    <message>
      <source>Attribute "%1" is not initialized.</source>
      <translation>Выберите острые углы на контуре.</translation>
    </message>
    <message>
      <source>Fillet the specified corners of the wire</source>
      <translation>Сгладить выбранные узлы контура</translation>
    </message>
  </context>
  <context>
    <name>Fillet1D:main_vertices:FeaturesPlugin_ValidatorFillet1DSelection</name>
    <message>
      <source>Selected vertex is not a wire corner</source>
      <translation>Выбранный узел не принадлежит контуру</translation>
    </message>
    <message>
      <source>Selected shape is not a vertex</source>
      <translation>Выбранный объект не является узлом</translation>
    </message>
    <message>
      <source>Selected vertex does not exist in the wire</source>
      <translation>Выбранный узел не принадлежит контуру</translation>
    </message>
    <message>
      <source>Vertex should be shared between 2 edges exactly</source>
      <translation>Узел должен примыкать ровно к двум рёбрам</translation>
    </message>
    <message>
      <source>Error: Edges are not planar</source>
      <translation>Ошибка: рёбра не лежат на одной плоскости</translation>
    </message>
    <message>
      <source>Error: Edges are tangent</source>
      <translation>Ошибка: рёбра касаются друг друга</translation>
    </message>
  </context>
  <context>
    <name>Fillet1D:creation_method</name>
    <message>
      <source>Wires</source>
      <translation>Контуры</translation>
    </message>
    <message>
      <source>Vertices</source>
      <translation>Узлы</translation>
    </message>
  </context>
  <context>
    <name>Fillet1D:radius</name>
    <message>
      <source>Radius</source>
      <translation>Радиус</translation>
    </message>
    <message>
      <source>Fillet radius</source>
      <translation>Радиус сглаживания</translation>
    </message>
    <message>
      <source>Attribute "%1" is not initialized.</source>
      <translation>Задайте радиус сглаживания.</translation>
    </message>
  </context>
  <context>
    <name>Fillet1D:radius:GeomValidators_Positive</name>
    <message>
      <source>Value is too small.</source>
      <translation>Значение радиуса слишком мало.</translation>
    </message>
  </context>

</TS>
