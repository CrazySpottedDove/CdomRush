#!/bin/bash
# filepath: \home\dove\CdomRush\scripts\synchronize.sh
# This script synchronizes the local repository with the remote repository.

# 颜色定义
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
RED='\033[0;31m'
NC='\033[0m'

print_info() { echo -e "${GREEN}[INFO]${NC} $1"; }
print_warning() { echo -e "${YELLOW}[WARNING]${NC} $1"; }
print_error() { echo -e "${RED}[ERROR]${NC} $1"; }

# 检查当前分支
current_branch=$(git branch --show-current)

# 如果不在master分支，先切换到master
if [ "$current_branch" != "master" ]; then
    print_warning "当前在分支 $current_branch，切换到master..."
    git checkout master
fi

# 获取远程更新
print_info "获取远程更新..."
git fetch origin

# 拉取远程master到本地master
print_info "同步远程master到本地..."
if git pull origin master; then
    print_info "同步完成！本地master已是最新"
else
    print_error "同步失败"
    exit 1
fi

# 显示最新提交
print_info "最新提交："
git log --oneline -3