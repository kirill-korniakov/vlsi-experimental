#include "HBaseDesignItem.h"
#include "HDesign.h"

void InitLD(LDesign*& m_ld, HDesign*& m_hd) {
    m_ld = &m_hd->_Design;
}
