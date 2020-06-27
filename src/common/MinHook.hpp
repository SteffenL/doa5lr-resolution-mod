#pragma once

class Hook {
public:
    Hook(void* pTarget, void* pDetour);
    ~Hook();
    void enable();
    void disable();
    void* getOriginal() const;

private:
    void* m_pTarget{};
    void* m_pDetour{};
    void* m_pOriginal{};
};

class MinHook {
public:
    MinHook();
    ~MinHook();
    Hook create(void* pTarget, void* pDetour);
};
