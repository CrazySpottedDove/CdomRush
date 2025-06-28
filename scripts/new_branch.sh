#!/bin/bash
# filepath: \home\dove\CdomRush\scripts\new_branch.sh
# This script creates a new branch for development.

# 颜色定义
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
RED='\033[0;31m'
NC='\033[0m'

print_info() { echo -e "${GREEN}[INFO]${NC} $1"; }
print_warning() { echo -e "${YELLOW}[WARNING]${NC} $1"; }
print_error() { echo -e "${RED}[ERROR]${NC} $1"; }

# 检查参数
if [ $# -ne 1 ]; then
    print_error "用法: $0 <分支名>"
    print_info "示例: $0 feature/tower_upgrade"
    exit 1
fi

branch_name=$1

# 检查分支名是否合法
if [[ ! $branch_name =~ ^[a-zA-Z0-9_-]+$ ]]; then
    print_error "分支名只能包含字母、数字、下划线和横线"
    exit 1
fi

# 检查是否已存在同名分支
if git show-ref --verify --quiet refs/heads/$branch_name; then
    print_error "分支 $branch_name 已存在"
    print_info "现有分支:"
    git branch
    exit 1
fi

# 确保在master分支
current_branch=$(git branch --show-current)
if [ "$current_branch" != "master" ]; then
    print_info "切换到master分支..."
    git checkout master
fi

# 确保master是最新的
print_info "更新master分支..."
git pull origin master

# 创建新分支
print_info "创建新分支: $branch_name"
git checkout -b $branch_name

print_info "成功创建并切换到分支: $branch_name"
print_info "现在可以开始开发了！"
print_warning "开发完成后使用 merge_branch.sh 合并到master"