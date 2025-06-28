#!/bin/bash
# filepath: \home\dove\CdomRush\scripts\merge_branch.sh
# This script merges current branch to master and pushes to remote.

# 颜色定义
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
RED='\033[0;31m'
NC='\033[0m'

print_info() { echo -e "${GREEN}[INFO]${NC} $1"; }
print_warning() { echo -e "${YELLOW}[WARNING]${NC} $1"; }
print_error() { echo -e "${RED}[ERROR]${NC} $1"; }

# 获取当前分支
current_branch=$(git branch --show-current)

# 检查是否在master分支
if [ "$current_branch" = "master" ]; then
    print_error "已在master分支，无法合并自己"
    exit 1
fi

print_info "当前分支: $current_branch"

# 检查是否有未提交的更改
if ! git diff-index --quiet HEAD --; then
    print_error "有未提交的更改，请先提交"
    print_info "未提交的文件:"
    git status --short
    exit 1
fi

# 确认合并
echo "即将执行以下操作:"
echo "1. 切换到master分支"
echo "2. 更新master到最新"
echo "3. 合并 $current_branch 到master"
echo "4. 推送master到远程"
echo "5. 删除本地分支 $current_branch"
echo
read -p "确认继续？(y/N): " -r
if [[ ! $REPLY =~ ^[Yy]$ ]]; then
    print_info "操作取消"
    exit 0
fi

# 切换到master
print_info "切换到master分支..."
git checkout master

# 更新master
print_info "更新master分支..."
git pull origin master

# 合并分支
print_info "合并分支 $current_branch 到master..."
if git merge $current_branch; then
    print_info "分支合并成功"
else
    print_error "合并失败，请手动解决冲突"
    exit 1
fi

# 推送到远程
print_info "推送master到远程..."
if git push origin master; then
    print_info "推送成功"
else
    print_error "推送失败"
    exit 1
fi

# 删除本地分支
print_info "删除本地分支 $current_branch..."
git branch -d $current_branch

print_info "所有操作完成！"
print_info "分支 $current_branch 已成功合并到master并推送到远程"