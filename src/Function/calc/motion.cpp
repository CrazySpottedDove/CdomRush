#include "Function/calc/motion.h"
#include "Manager/pathManager/pathManager.h"
#include "Manager/store/store.h"
#include "Model/enemies/enemies.h"
#include "Model/templates/unit.h"

void calc::enemy_move_tick(const Store& store, Enemy& self){
    const PathManager& path_manager = store.path_manager;
    const PathInfo& path_info = self.path_info;
    const SubPath& sub_path = path_manager.GetSubPath(path_info);
    
}