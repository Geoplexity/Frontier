#ifndef ENTITIES_DEG_VALUES_H
#define ENTITIES_DEG_VALUES_H

namespace ffnx::mfa {

//class that stores one set of degvalues exactly as described in Class Vertex above
    class DegValues {

    private:

        float degValues[8];

    public:

        DegValues() {
            int i;

            for (i = 0; i < 8; i++) degValues[i] = 0;
        }

        void setValue(int i, float value) { degValues[i] = value; }

        float returnValue(int i) { return degValues[i]; }

        DegValues &operator=(const DegValues &deg);
    };

    DegValues &DegValues::operator=(const DegValues &deg) {
        int i;

        if (this != &deg) {
            for (i = 0; i < 8; i++)
                degValues[i] = deg.degValues[i];

        }
        return *this;
    }
}

#endif