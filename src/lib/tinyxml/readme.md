XMLElement 是 TinyXML2 库中用于表示 XML 元素的类，它提供了一系列方法来操作和访问 XML 元素。以下是 XMLElement 类的一些基本方法：

1. ### 访问元素名称
-   `const char* Name()` const：返回元素的名称。
-   `void SetName(const char* name)`：设置元素的名称。
2. ### 访问和设置元素文本
-   `const char* GetText()` const：返回元素的文本内容。
-   `bool QueryIntText(int* outValue)` const：将元素的文本内容转换为 int 类型并存储在 outValue 中。若转换成功返回 XML_SUCCESS，否则返回错误码。
-   `bool QueryUnsignedText(unsigned* outValue)` const：将元素的文本内容转换为 unsigned 类型并存储在 outValue 中。
-   `bool QueryDoubleText(double* outValue)` const：将元素的文本内容转换为 double 类型并存储在 outValue 中。
-   `void SetText(const char* text)`：设置元素的文本内容。
-   `void SetText(int text)`：设置元素的文本内容为 int 类型的值。
-   `void SetText(double text)`：设置元素的文本内容为 double 类型的值。
3. ### 访问和设置属性
-   `const XMLAttribute* FirstAttribute()` const
-   `const XMLAttribute* NextAttribute()` const （这实际上是 XMLAttribute 类的方法，与 FirstAttribute() 配合使用来遍历属性）
-   `const char* Attribute(const char* name)` const：返回指定名称的属性值。
-   `bool QueryIntAttribute(const char* name, int* outValue)` const：将指定名称的属性值转换为 int 类型并存储在 outValue 中。
-   `bool QueryUnsignedAttribute(const char* name, unsigned* outValue)` const：将指定名称的属性值转换为 unsigned 类型并存储在 outValue 中。
-   `bool QueryDoubleAttribute(const char* name, double* outValue)` const：将指定名称的属性值转换为 double 类型并存储在 outValue 中。
-   `void SetAttribute(const char* name, const char* value)`：设置指定名称的属性值。
-   `void SetAttribute(const char* name, int value)`：设置指定名称的属性值为 int 类型的值。
-   `void SetAttribute(const char* name, double value)`：设置指定名称的属性值为 double 类型的值。
-   `void DeleteAttribute(const char* name)`：删除指定名称的属性。
4. ### 访问子节点
-   `XMLElement* FirstChildElement()` const：返回第一个子元素。
-   `XMLElement* FirstChildElement(const char* name)` const：返回第一个指定名称的子元素。
-   `XMLElement* NextSiblingElement()` const：返回下一个兄弟元素。
-   `XMLElement* NextSiblingElement(const char* name)` const：返回下一个指定名称的兄弟元素。
-   `XMLElement* LastChildElement()` const：返回最后一个子元素。
-   `XMLElement* LastChildElement(const char* name)` const：返回最后一个指定名称的子元素。
5. ### 操作子节点
-   `XMLNode* InsertEndChild(XMLNode* node)`：在元素的末尾插入一个子节点。
-   `XMLNode* InsertFirstChild(XMLNode* node)`：在元素的开头插入一个子节点。
-   `XMLNode* InsertAfterChild(XMLNode* reference, XMLNode* node)`：在指定的参考节点之后插入一个子节点。
-   `void DeleteChild(XMLNode* node)`：删除指定的子节点。
-   `void DeleteChildren()`：删除所有子节点。