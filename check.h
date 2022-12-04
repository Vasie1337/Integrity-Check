#pragma once
#include <iostream>
#include <Windows.h>
#include <vector>
#include <nmmintrin.h>

struct section {
    std::uint8_t* name = {};
    void* address = {};
    std::uint32_t checksum = {};

    bool operator==(section& other)
    {
        return this->checksum == other.checksum;
    }
};

std::uintptr_t module = reinterpret_cast<std::uintptr_t>(GetModuleHandle(nullptr));

__forceinline std::uint32_t crc32(void* data, std::size_t size)
{
    std::uint32_t result = {};

    for (std::size_t index = {}; index < size; ++index)
        result = _mm_crc32_u32(result, reinterpret_cast<std::uint8_t*>(data)[index]);

    return result;
}

std::vector<section> get_sections()
{
    std::vector<section> sections = {};

    PIMAGE_DOS_HEADER dosheader = reinterpret_cast<PIMAGE_DOS_HEADER>(module);
    PIMAGE_NT_HEADERS nt_headers = reinterpret_cast<PIMAGE_NT_HEADERS>(module + dosheader->e_lfanew);

    PIMAGE_SECTION_HEADER section = IMAGE_FIRST_SECTION(nt_headers);

    for (int i = 0; i < nt_headers->FileHeader.NumberOfSections; i++, section++)
    {
        void* address = reinterpret_cast<void*>(module + section->VirtualAddress);
        sections.push_back({ section->Name, address, crc32(address, section->Misc.VirtualSize) });
    }
    return sections;
}

__forceinline std::vector<section> compare_sections(std::vector<section> sections1, std::vector<section> sections2)
{
    std::vector<section> result = {};

    for (std::size_t index = {}; index < sections1.size(); index++)
        if (!(sections1[index] == sections2[index]))
            result.push_back(sections2[index]);

    return result;
}

/// <summary>
/// checks .text integrity.
/// </summary>
/// <returns>returns true if everything is right, else false.</returns>
__forceinline bool check_integrity(std::vector<section> _cached)
{
    const std::vector<section> sections = compare_sections(_cached, get_sections());
    for (const section& section : sections)
    {
        std::string name(reinterpret_cast<char const*>(section.name));
        if (name == ".text")
            return false;
        return true;
    }
}