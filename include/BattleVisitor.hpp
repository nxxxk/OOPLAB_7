#pragma once

#include "IVisitor.hpp"

struct OrcVisitor : public IVisitor {
    using IVisitor::IVisitor;
    bool visit(Orc& defender) override;
    bool visit(Bear& defender) override;
    bool visit(Squirrel& defender) override;
};

struct BearVisitor : public IVisitor {
    using IVisitor::IVisitor;
    bool visit(Orc& defender) override;
    bool visit(Bear& defender) override;
    bool visit(Squirrel& defender) override;
};

struct SquirrelVisitor : public IVisitor {
    using IVisitor::IVisitor;
    bool visit(Orc& defender) override;
    bool visit(Bear& defender) override;
    bool visit(Squirrel& defender) override;
};
