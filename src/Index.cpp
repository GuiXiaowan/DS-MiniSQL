#include "index.h"

pIndexResult Index::select(pAttrInfo *attrInfo, char *value)
{
    // 根据字段信息获得字段值的类型
    bool isInt = 0;
    if (attrInfo->type == INT)
        isInt = 1;
    else
        isInt = 0;

    // 根据索引名找到索引文件中的索引头
    indexHead idxHd;
    indexNode curNode = idxHd.firstNode;

    // 字段类型是整型
    if (isInt)
    {
        int iValue = atoi(value);
        int curValue = atoi(curNode.value); 

        // 查找该索引项
        while (curNode != NULL)
        {
            curValue = atoi(curNode.value);

            // 找到了要提取的索引项
            if (curValue == iValue)
            {
                selectResult.pageNum = curNode.pageNum;
                selectResult.pageOffset = curNode.pageOffset;
                return &selectResult;
            }

            curNode = curNode.nextNode;
        }

        // 没有找到该索引项
        if (curNode == NULL)
        {
            // 报错
        }

    }
    // 字段类型是字符串
    else
    {
        char *curValue = curNode.value; 

        // 查找该索引项
        while (curNode != NULL)
        {
            curValue = curNode.value;

            // 找到了要提取的索引项
            if (curValue == value)
            {
                selectResult.pageNum = curNode.pageNum;
                selectResult.pageOffset = curNode.pageOffset;
                return &selectResult;
            }

            curNode = curNode.nextNode;
        }

        // 没有找到该索引项
        if (curNode == NULL)
        {
            // 报错
        }
    }
}

void Index::create(pAttrInfo *attrInfo)
{
    indexHead idxHd = new indexHead;
    idxhd.pAttr = *(attrInfo);
    idxhd.firstNode = NULL;
}

void Index::insert(pAttrInfo *attrInfo, char *value)
{
    // 根据字段信息获得字段值的类型
    bool isInt = 0;
    if (attrInfo->type == INT)
        isInt = 1;
    else
        isInt = 0;

    // 根据 Record 的信息获得 页号 和 页偏移
    unsigned pgNum;
    unsigned long pgOffset;

    // 建立新索引项
    indexNode idxNode = new indexNode;
    memcpy(idxNode.value, value, strlen(value));
    idxNode.pageNum = pgNum;
    idxNode.pageOffset = pgOffset;
    idxNode.nextNode = NULL;

    // 根据索引名找到索引文件中的索引头
    indexHead idxHd;

    // 若索引还是空的，那么就创建第一个索引
    if (idxHd.firstNode == NULL)
    {
        idxHd.firstNode = idxNode;
        return;
    }

    // 若索引不是空的，那么就找到合适的地方插入
    if (idxHd.firstNode != NULL)
    {
        indexNode curNode = idxHd.firstNode;
        // 字段类型是整型
        if (isInt)
        {
            int iValue = atoi(value);
            int curValue = atoi(curNode.value); 

            // 已有相同值，无法插入
            if (curValue == iValue)
            {
                return;
            }

            // 第一个索引项比插入索引项大，插入索引项变为第一项
            if (curValue > iValue)
            {
                idxHd.firstNode = idxNode;
                idxNode.nextNode = curNode;
                return;
            }

            // 第一个索引项比插入索引项小，普通情况
            while (curValue < iValue)
            {
                // 插到最后
                if (curNode.nextNode == NULL)
                {
                    curNode.nextNode = idxNode;
                    return;
                }

                // 插到中间
                else if (atoi(curNode.nextNode.value) > iValue)
                {
                    indexNode nextNode = curNode.nextNode;
                    curNode.nextNode = idxNode;
                    idxNode.nextNode = nextNode;
                    return;
                }

                curNode = curNode.nextNode;
                curValue = atoi(curNode.value);
            }
        }
        // 字段类型是字符串
        else
        {
            char *curValue = curNode.value;

            // 已有相同值，无法插入
            if (curValue == value)
            {
                return;
            }

            // 第一个索引项比插入索引项大，插入索引项变为第一项
            if (curValue > value)
            {
                idxHd.firstNode = idxNode;
                idxNode.nextNode = curNode;
                return;
            }

            // 第一个索引项比插入索引项小，普通情况
            while (curValue < value)
            {
                // 插到最后
                if (curNode.nextNode == NULL)
                {
                    curNode.nextNode = idxNode;
                    return;
                }

                // 插到中间
                else if (curNode.nextNode.value > curValue)
                {
                    indexNode nextNode = curNode.nextNode;
                    curNode.nextNode = idxNode;
                    idxNode.nextNode = nextNode;
                    return;
                }

                curNode = curNode.nextNode;
                curValue = curNode.value;
            }
        }
    }
}

void Index::delete(pAttrInfo *attrInfo, char *value)
{
    // 根据字段信息获得字段值的类型
    bool isInt = 0;
    if (attrInfo->type == INT)
        isInt = 1;
    else
        isInt = 0;

    // 根据索引名找到索引文件中的索引头
    indexHead idxHd;
    indexNode curNode = idxHd.firstNode;

    // 字段类型是整型
    if (isInt)
    {
        int iValue = atoi(value);
        int curValue = atoi(curNode.value); 

        // 第一项即该索引项，删除之
        if (curValue == iValue)
        {
            indexNode nextNode = curNode.nextNode;
            idxHd.firstNode = nextNode;
            delete curNode;
            return;
        }

        // 否则查找该索引项
        while (true)
        {
            // 如果下一个索引项是要找的索引项
            if (atoi(curNode.nextNode.value) == iValue)
            {
                indexNode prevNode = curNode;
                indexNode curNode = curNode.nextNode;
                indexNode nextNode = curNode.nextNode;

                delete curNode;
                prevNode.nextNode = nextNode;
                return;
            }

            curNode = curNode.nextNode;
            curValue = atoi(curNode.value);
        }
    }
    // 字段类型是字符串
    else
    {
        char *curValue = curNode.value; 

        // 第一项即该索引项，删除之
        if (curValue == value)
        {
            indexNode nextNode = curNode.nextNode;
            idxHd.firstNode = nextNode;
            delete curNode;
            return;
        }

        // 否则查找该索引项
        while (true)
        {
            // 如果下一个索引项是要找的索引项
            if (curNode.nextNode.value == value)
            {
                indexNode prevNode = curNode;
                indexNode curNode = curNode.nextNode;
                indexNode nextNode = curNode.nextNode;

                delete curNode;
                prevNode.nextNode = nextNode;
                return;
            }

            curNode = curNode.nextNode;
            curValue = curNode.value;
        }
    }
}

void Index::update(pAttrInfo *attrInfo, char *value, char *newValue)
{
    // 根据字段信息获得字段值的类型
    bool isInt = 0;
    if (attrInfo->type == INT)
        isInt = 1;
    else
        isInt = 0;

    // 根据索引名找到索引文件中的索引头
    indexHead idxHd;
    indexNode curNode = idxHd.firstNode;

    // 字段类型是整型
    if (isInt)
    {
        int iValue = atoi(value);
        int curValue = atoi(curNode.value); 

        // 查找该索引项
        while (true)
        {
            // 找到了要更新的索引项
            if (curValue == iValue)
            {
                memset(curNode.value, 0, strlen(curNode.value));
                memcpy(curNode.value, newValue, strlen(newValue));
                return;
            }

            curNode = curNode.nextNode;
            curValue = atoi(curNode.value);
        }
    }
    // 字段类型是字符串
    else
    {
        char *curValue = curNode.value; 

        // 查找该索引项
        while (true)
        {
            // 找到了要更新的索引项
            if (curValue == value)
            {
                memset(curNode.value, 0, strlen(curNode.value));
                memcpy(curNode.value, newValue, strlen(newValue));
                return;
            }

            curNode = curNode.nextNode;
            curValue = curNode.value;
        }
    }
}

Index::Index()
{

}

Index::~Index()
{

}