#pragma once

#include <Core/Block.h>
#include <Processors/Formats/IRowInputFormat.h>
#include <Processors/Formats/RowInputFormatWithNamesAndTypes.h>


namespace DB
{

class ReadBuffer;


/** A stream for inputting data in a binary line-by-line format.
  */
class BinaryRowInputFormat : public RowInputFormatWithNamesAndTypes
{
public:
    BinaryRowInputFormat(ReadBuffer & in_, Block header, Params params_, bool with_names_, bool with_types_, const FormatSettings & format_settings_);

    String getName() const override { return "BinaryRowInputFormat"; }

    /// RowInputFormatWithNamesAndTypes implements logic with DiagnosticInfo, but
    /// in this format we cannot provide any DiagnosticInfo, because here we have
    /// just binary data.
    std::string getDiagnosticInfo() override { return {}; }

private:
    bool readField(IColumn & column, const DataTypePtr & type, const SerializationPtr & serialization, bool is_last_file_column, const String & column_name) override;
    void skipField(size_t file_column) override;

    void skipNames() override;
    void skipTypes() override;
    void skipHeaderRow();

    std::vector<String> readNames() override;
    std::vector<String> readTypes() override;
    std::vector<String> readHeaderRow();

    /// Data types read from input data.
    DataTypes read_data_types;
    UInt64 read_columns = 0;
};

}
