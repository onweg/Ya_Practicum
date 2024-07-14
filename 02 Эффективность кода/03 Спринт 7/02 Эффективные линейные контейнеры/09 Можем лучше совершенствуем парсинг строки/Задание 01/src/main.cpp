class Translator {
public:
    void Add(string_view source, string_view target);
    string_view TranslateForward(string_view source) const;
    string_view TranslateBackward(string_view target) const;

private:
    // ...
}