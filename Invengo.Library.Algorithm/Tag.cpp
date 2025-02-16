#include "pch.h"
#include "Tag.h"
#include <cstring> // For strncpy_s
#include <vector>
#include <unordered_map>
#include <string>
#include <numeric>



//strncpy_s：
//
//当你需要安全地复制一个固定长度的字符串时，例如从用户输入中获取字符串并存储到一个固定大小的缓冲区中。
//强调安全性，避免缓冲区溢出。
//snprintf：
//
//当你需要将各种类型的数据格式化为一个字符串时，例如将数值转换为字符串、拼接多个字符串。
//需要灵活的格式化输出。


class Tag {
public:
    Tag(int antenna, const char* epc, double rssi, int readcount,const wchar_t* name) 
        : antenna(antenna), 
        rssi(rssi),
        readcount(readcount){
        strncpy_s(this->epc, epc, sizeof(this->epc) - 1);
        this->epc[sizeof(this->epc) - 1] = '\0'; // Ensure null termination

        wcsncpy_s(this->name, name, sizeof(this->name) / sizeof(this->name[0]) - 1);
        this->name[sizeof(this->name) / sizeof(this->name[0]) - 1] = L'\0'; // 确保 null 终止
    }
    int GetAnt() const { return antenna; }
    const char* GetEpc() const { return epc; }
    double GetRssi() const { return rssi; }
    int GetReadCount() const { return readcount; }
    const wchar_t* GetName() const { return name; }

private:
    int antenna;
    char epc[100];
    double rssi;
    int readcount;
    wchar_t name[100]; //含有中文
};

extern "C" {

    MYLIBRARY_API Tag* CreateTag(int antenna, const char* epc, double rssi, int readcount, const wchar_t* name) {
        return new Tag(antenna, epc, rssi, readcount, name);
    }

    MYLIBRARY_API int GetTagAnt(Tag* Tag) {
        return Tag ? Tag->GetAnt() : -1;
    }

    MYLIBRARY_API const char* GetTagEpc(Tag* Tag) {
        return Tag ? Tag->GetEpc() : "";
    }

    MYLIBRARY_API double GetTagRssi(Tag* Tag) {
        return Tag ? Tag->GetRssi() : -1.0f;
    }

    MYLIBRARY_API int GetTagReadCount(Tag* Tag) {
        return Tag ? Tag->GetReadCount() : -1;
    }

    MYLIBRARY_API const wchar_t* GetTagName(Tag* Tag) {
        return Tag ? Tag->GetName() : L"";
    }

    MYLIBRARY_API void DestroyTag(Tag* Tag) {
        delete Tag;
    }

    MYLIBRARY_API std::vector<Tag*>* CreateTags() {
        return new std::vector<Tag*>();
    }

    MYLIBRARY_API void AddTag(std::vector<Tag*>* collection, Tag* Tag) {
        if (collection && Tag) {
            collection->push_back(Tag);
        }
    }

    MYLIBRARY_API Tag* GetTag(std::vector<Tag*>* collection, int index) {
        if (collection && index >= 0 && index < collection->size()) {
            return (*collection)[index];
        }
        return nullptr;
    }

    MYLIBRARY_API int GetTagsSize(std::vector<Tag*>* collection) {
        return collection ? static_cast<int>(collection->size()) : 0;
    }

    MYLIBRARY_API void DestroyTags(std::vector<Tag*>* collection) {
        if (collection) {
            for (Tag* Tag : *collection) {
                delete Tag;
            }
            delete collection;
        }
    }
    MYLIBRARY_API std::vector<Tag*>* AnalyzeTags(std::vector<Tag*>* collection, double min, double max)
    {
        if (collection) {
            auto* newtags = new  std::vector<Tag*>();
            std::unordered_map<std::string, std::vector<Tag*>> map_epc_tags;//标签按照EPC分组
            for (const auto& tag : *collection) {
                map_epc_tags[tag->GetEpc()].push_back(tag);
            }
            for (const auto& epc_kv : map_epc_tags) {
                auto epc = epc_kv.first;                                    //单个epc
                auto tags_epc = epc_kv.second;                              //单个EPC标签集合
                std::unordered_map<int, std::vector<Tag*>> map_ant_tags;    //单个EPC按照天线分组
                for (const auto& epc_tag : tags_epc) {
                    map_ant_tags[epc_tag->GetAnt()].push_back(epc_tag);
                }
                int ant_r = 1;
                double rssi_r = -std::numeric_limits<double>::infinity();
                int readcount_r = 0;
                const wchar_t* name_r = L"";

                for (const auto& ant_kv : map_ant_tags) {
                    auto ant = ant_kv.first;                                //单个天线
                    auto tags_ant = ant_kv.second;                          //单个天线的集合
                    //得到rssi 总和
                    double totalRssi = std::accumulate(tags_ant.begin(), tags_ant.end(), 0.0,
                        [](double sum, const Tag* tag) {
                            return sum + tag->GetRssi();
                        });
                    double avRssi = (totalRssi / tags_ant.size());


                    int totalCount= std::accumulate(tags_ant.begin(), tags_ant.end(), 0.0,
                        [](double sum, const Tag* tag) {
                            return sum + tag->GetReadCount();
                        });
                    int avCount = totalCount / tags_ant.size();

                    if (avRssi > rssi_r) {
                        ant_r = ant;
                        rssi_r = avRssi;
                        readcount_r = avCount;
                        name_r = tags_ant[0]->GetName();
                    }
                }
                if (rssi_r < min)
                    continue;
                if (rssi_r > max)
                    continue;
                Tag* tag_r = new Tag(ant_r, epc.c_str(), rssi_r, readcount_r, name_r);
                newtags->push_back(tag_r);
            }
            return newtags;
        }
        return nullptr;
    }
}
