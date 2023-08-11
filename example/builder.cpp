/*建造者模式 -- 使用多个对象一步步构建成一个复杂对象*/
#include <iostream>
#include <string>
#include <memory>

// 抽象笔记本电脑类
class Computer
{
public:
    Computer()
    {}
    void SetBoard(const std::string &board)
    {
        _board = board;
    }
    void SetDisplay(const std::string &display)
    {
        _display = display;
    }
    void ShowParamaters()
    {
        std::string param = "Computer Paramaters:\n";
        param += "\tBoard: " + _board + "\n";
        param += "\tDisplay: " + _display + "\n";
        param += "\tOS: " + _os + "\n";
        std::cout << param << std::endl;
    }
    virtual void SetOS() = 0;
protected:
    std::string _board;
    std::string _display;
    std::string _os;
};
// 具体的苹果笔记本
class MacBook : public Computer
{
public:
    void SetOS() override
    {
        // public继承 子类不能访问父类的private成员，要改为protected
        _os = "Mac OS X12";
    }  
};
// 抽象建造者
class Builder
{
public:
    virtual void BuildBoard(const std::string &board) = 0; // = 0 表示纯虚函数，只能给子类继承，不能实例化
    virtual void BuildDisplay(const std::string &display) = 0;
    virtual void BuildOS() = 0;
    virtual std::shared_ptr<Computer> build() = 0;
};
// 苹果笔记本建造者
class MacBookBuilder : public Builder
{
public:
    MacBookBuilder()
        : _computer(new MacBook)
    {}
    void BuildBoard(const std::string &board)
    {
        _computer->SetBoard(board);
    }
    void BuildDisplay(const std::string &display)
    {
        _computer->SetDisplay(display);
    }
    void BuildOS()
    {
        _computer->SetOS();
    }
    std::shared_ptr<Computer> build()
    {
        return _computer;
    }
private:
    std::shared_ptr<Computer> _computer;
};
// 指挥者 - 决定构造顺序，参数
class Director
{
public:
    Director(Builder *builder)
        : _builder(builder)
    {}
    void construct(const std::string &board, const std::string &display)
    {
        _builder->BuildBoard(board);
        _builder->BuildDisplay(display);
        _builder->BuildOS();
    }
private:
    std::shared_ptr<Builder> _builder;
};

int main()
{
    Builder *builder = new MacBookBuilder();
    std::unique_ptr<Director> director(new Director(builder));
    director->construct("华硕主板", "三星显示器");
    std::shared_ptr<Computer> computer = builder->build();
    computer->ShowParamaters();
    return 0;
}