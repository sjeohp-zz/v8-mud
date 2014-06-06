#ifndef Mud_Serializable_h
#define Mud_Serializable_h

class Serializable
{
public:
    Serializable(){}
    virtual ~Serializable(){}

    virtual void serialize(std::ostream& os) = 0;
    virtual void deserialize(std::istream& is) = 0;
};

#endif