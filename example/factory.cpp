#include <iostream>
#include <memory>

class Fruit
{
public:
    virtual void name() = 0;
};

class Apple : public Fruit
{
public:
    void name() override
    {
        std::cout << "我是一个苹果.." << std::endl;
    }
};

class Banana : public Fruit
{
public:
    void name() override
    {
        std::cout << "我是一个香蕉.." << std::endl;
    }
};

class Animal
{
public:
    virtual void name() = 0;
};

class Dog : public Animal
{
public:
    void name() override
    {
        std::cout << "我是一只小狗.." << std::endl;
    }
};

class Cat : public Animal
{
public:
    void name() override
    {
        std::cout << "我是一只小猫.." << std::endl;
    }
};

/*简单工厂模式 -- 由一个工厂对象通过类型决定创建出来指定产品类的实例*/
// class FruitFactory
// {
// public:
//     static std::shared_ptr<Fruit> create(const std::string &name)
//     {
//         if(name == "苹果")
//         {
//             return std::make_shared<Apple>();
//         }
//         else
//         {
//             return std::make_shared<Banana>();
//         }
//     }
// };

/*工厂方法模式 -- 在简单工厂模式下新增多个工厂，多个产品，每个产品对应一个工厂。*/
// class FruitFactory
// {
// public:
//     virtual std::shared_ptr<Fruit> create() = 0;
// };

// class AppleFactory : public FruitFactory
// {
// public:
//     std::shared_ptr<Fruit> create() override
//     {
//         return std::make_shared<Apple>();
//     }
// };

// class BananaFactory : public FruitFactory
// {
// public:
//     std::shared_ptr<Fruit> create() override
//     {
//         return std::make_shared<Banana>();
//     }
// };

/*抽象工厂模式 -- 围绕一个超级工厂创建其他工厂。每个生成的工厂按照工厂模式提供对象*/
class Factory
{
public:
    virtual std::shared_ptr<Fruit> getFruit(const std::string &name) = 0;
    virtual std::shared_ptr<Animal> getAnimal(const std::string &name) = 0;
};

class FruitFactory : public Factory
{
public:
    std::shared_ptr<Animal> getAnimal(const std::string &name) override
    {
        return std::shared_ptr<Animal>();
    }
    std::shared_ptr<Fruit> getFruit(const std::string &name) override
    {
        if (name == "苹果")
        {
            return std::make_shared<Apple>();
        }
        else
        {
            return std::make_shared<Banana>();
        }
    }
};

class AnimalFactory : public Factory
{
public:
    std::shared_ptr<Fruit> getFruit(const std::string &name) override
    {
        return std::shared_ptr<Fruit>();
    }
    std::shared_ptr<Animal> getAnimal(const std::string &name) override
    {
        if (name == "小狗")
        {
            return std::make_shared<Dog>();
        }
        else
        {
            return std::make_shared<Cat>();
        }
    }
};

class FactoryProducer
{
public:
    static std::shared_ptr<Factory> create(const std::string &name)
    {
        if (name == "水果")
        {
            return std::make_shared<FruitFactory>();
        }
        else
        {
            return std::make_shared<AnimalFactory>();
        }
    }
};
int main()
{
    // std::shared_ptr<Fruit> fruit = FruitFactory::create("苹果");
    // fruit->name();
    // fruit = FruitFactory::create("香蕉");
    // fruit->name();

    // std::shared_ptr<FruitFactory> ff(new AppleFactory());
    // std::shared_ptr<Fruit> fruit = ff->create();
    // fruit->name();
    // ff.reset(new BananaFactory());
    // fruit=ff->create();
    // fruit->name();

    std::shared_ptr<Factory> ff = FactoryProducer::create("水果");
    std::shared_ptr<Fruit> fruit = ff->getFruit("苹果");
    fruit->name();
    fruit = ff->getFruit("香蕉");
    fruit->name();

    std::shared_ptr<Factory> af = FactoryProducer::create("动物");
    std::shared_ptr<Animal> animal = af->getAnimal("小狗");
    animal->name();
    animal = af->getAnimal("小猫");
    animal->name();
    return 0;
}