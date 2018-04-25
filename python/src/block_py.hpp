//
//  block_py.hpp
//  blocksci
//
//  Created by Harry Kalodner on 3/14/18.
//

#ifndef block_py_h
#define block_py_h

#include <blocksci/chain/block.hpp>
#include <blocksci/chain/algorithms.hpp>

#include <pybind11/pybind11.h>
#include <pybind11/chrono.h>

#include <range/v3/view/any_view.hpp>

void init_block(pybind11::module &m);

struct AddBlockMethods {
    using FuncDoc = std::function<const char *(std::string)>;

    FuncDoc func2;

    AddBlockMethods(FuncDoc funcDoc_) : func2(std::move(funcDoc_)) {}

    template <typename Class, typename FuncApplication>
    void operator()(Class &cl, FuncApplication func) {
        namespace py = pybind11;
        using namespace blocksci;
        cl
        .def_property_readonly("txes", func([](const Block &block) -> ranges::any_view<Transaction, ranges::category::random_access> {
            return block;
        }), func2(R"docstring(
           Returns a range of all of the txes in the block.
           
           :returns: AnyTxRange
           )docstring"))
        .def_property_readonly("inputs", func([](const Block &block) -> ranges::any_view<Input> {
            return inputs(block);
        }), func2(R"docstring(
           Returns a range of all of the inputs in the block.
           
           :returns: AnyInputRange
           )docstring"))
        
        .def_property_readonly("outputs", func([](const Block &block) -> ranges::any_view<Output> {
            return outputs(block);
        }), func2(R"docstring(
           Returns a range of all of the outputs in the block.
           
           :returns: AnyOutputRange
           )docstring"))
        .def_property_readonly("hash", func(&Block::getHash), func2("Hash of this block"))
        .def_property_readonly("version", func(&Block::version), func2("Protocol version specified in block header"))
        .def_property_readonly("timestamp", func(&Block::timestamp), func2("Creation timestamp specified in block header"))
        .def_property_readonly("time", func(&Block::getTime), func2("Datetime object created from creation timestamp"))
        .def_property_readonly("time_seen", func(&Block::getTimeSeen), func2("If recorded by the mempool recorder, the time that this block was first seen by your node"))
        .def_property_readonly("bits", func(&Block::bits), func2("Difficulty threshold specified in block header"))
        .def_property_readonly("nonce", func(&Block::nonce), func2("Nonce specified in block header"))
        .def_property_readonly("height", func(&Block::height), func2("Height of the block in the blockchain"))
        .def_property_readonly("coinbase_param", func([](const Block &block) -> py::bytes {
            return py::bytes(block.coinbaseParam());
        }), func2("Data contained within the coinbase transaction of this block"))
        .def_property_readonly("coinbase_tx", func(&Block::coinbaseTx), func2("Return the coinbase transaction in this block"))
        .def_property_readonly("size_bytes", func(&Block::totalSize), func2("Returns the total size of the block in bytes"))
        .def_property_readonly("fee", func(totalFee<Block>), func2("The sum of the transaction fees contained in this block"))
        .def_property_readonly("revenue", func([](const Block &block) -> int64_t {
            return totalOutputValue(block[0]);
        }), func2("Total reward received by the miner of this block"))
        .def_property_readonly("base_size", func(&Block::baseSize), func2("The size of the non-segwit data in bytes"))
        .def_property_readonly("total_size", func(&Block::totalSize), func2("The size all block data in bytes"))
        .def_property_readonly("virtual_size", func(&Block::virtualSize), func2("The weight of the block divided by 4"))
        .def_property_readonly("weight", func(&Block::weight), func2("Three times the base size plus the total size"))
        .def_property_readonly("input_value", func(totalInputValue<Block &>), func2("Returns the sum of the value of all of the inputs included in this block"))
        .def_property_readonly("output_value", func(totalOutputValue<Block &>), func2("Returns the sum of the value of all of the outputs included in this block"))
        .def_property_readonly("input_count", func(inputCount<Block &>), func2("Returns total number of inputs included in this block"))
        .def_property_readonly("output_count", func(outputCount<Block &>), func2("Returns total number of outputs included in this block"))
        ;
    }
};

#endif /* block_py_h */
