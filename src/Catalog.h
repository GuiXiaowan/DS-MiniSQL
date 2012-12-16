#ifndef CATALOG_H
#define CATALOG_H

#include "Interpreter.h"

/* 表与字段的结构 */
typedef struct {
    string name;        // 字段名
    bool isPrimary;     // 是否主键
    int length;         // 字段占字节数
    attrtype_t type;    // 类型（CHAR, INT）
    // bool isUnique;
} attr_t;

/* 表的结构 */
typedef struct 
{
    string name;        // 表名
    int attrNumber;     // 字段数
    int recordLength;   // 一条记录的字节数
    attr_t attributes[MAX_ATTR_NUM];    //字段
} table_t;

/***********************************************************/

class Catalog
{
public:
    bool tableExist(char *tableName);                           // 返回表是否存在
    bool attrExist(struct Information_t token);                 // 返回字段是否存在
    bool attrTypeCheck(struct Information_t token);             // 返回字段信息是否满足类型条件

    pAttrInfo findAttr(struct Information_t token);             // 返回要求的字段信息
    pAttrInfo getPrimaryAttrName(struct Information_t token);   // 返回主键的字段信息（然后去找索引）

    void create(struct Information_t token);                    // 建表
    void delete(struct Information_t token);                    // 删除表

    Catalog();
    ~Catalog();
private:
};

#endif