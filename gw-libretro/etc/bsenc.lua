local pl_lexer = [===[
--- Lexical scanner for creating a sequence of tokens from text.
-- `lexer.scan(s)` returns an iterator over all tokens found in the
-- string `s`. This iterator returns two values, a token type string
-- (such as 'string' for quoted string, 'iden' for identifier) and the value of the
-- token.
--
-- Versions specialized for Lua and C are available; these also handle block comments
-- and classify keywords as 'keyword' tokens. For example:
--
--    > s = 'for i=1,n do'
--    > for t,v in lexer.lua(s)  do print(t,v) end
--    keyword for
--    iden    i
--    =       =
--    number  1
--    ,       ,
--    iden    n
--    keyword do
--
-- See the Guide for further @{06-data.md.Lexical_Scanning|discussion}
-- @module pl.lexer

local yield,wrap = coroutine.yield,coroutine.wrap
local strfind = string.find
local strsub = string.sub
local append = table.insert

local function assert_arg(idx,val,tp)
    if type(val) ~= tp then
        error("argument "..idx.." must be "..tp, 2)
    end
end

local lexer = {}

local NUMBER1 = '^[%+%-]?%d+%.?%d*[eE][%+%-]?%d+'
local NUMBER2 = '^[%+%-]?%d+%.?%d*'
local NUMBER3 = '^0x[%da-fA-F]+'
local NUMBER4 = '^%d+%.?%d*[eE][%+%-]?%d+'
local NUMBER5 = '^%d+%.?%d*'
local IDEN = '^[%a_][%w_]*'
local WSPACE = '^%s+'
local STRING0 = [[^(['\"]).-\\%1]]
local STRING1 = [[^(['\"]).-[^\]%1]]
local STRING3 = "^((['\"])%2)" -- empty string
local PREPRO = '^#.-[^\\]\n'

local plain_matches,lua_matches,cpp_matches,lua_keyword,cpp_keyword

local function tdump(tok)
    return yield(tok,tok)
end

local function ndump(tok,options)
    if options and options.number then
        tok = tonumber(tok)
    end
    return yield("number",tok)
end

-- regular strings, single or double quotes; usually we want them
-- without the quotes
local function sdump(tok,options)
    if options and options.string then
        tok = tok:sub(2,-2)
    end
    return yield("string",tok)
end

-- long Lua strings need extra work to get rid of the quotes
local function sdump_l(tok,options,findres)
    if options and options.string then
        local quotelen = 3
        if findres[3] then
            quotelen = quotelen + findres[3]:len()
        end
        tok = tok:sub(quotelen,-1 * quotelen)
    end
    return yield("string",tok)
end

local function chdump(tok,options)
    if options and options.string then
        tok = tok:sub(2,-2)
    end
    return yield("char",tok)
end

local function cdump(tok)
    return yield('comment',tok)
end

local function wsdump (tok)
    return yield("space",tok)
end

local function pdump (tok)
    return yield('prepro',tok)
end

local function plain_vdump(tok)
    return yield("iden",tok)
end

local function lua_vdump(tok)
    if lua_keyword[tok] then
        return yield("keyword",tok)
    else
        return yield("iden",tok)
    end
end

local function cpp_vdump(tok)
    if cpp_keyword[tok] then
        return yield("keyword",tok)
    else
        return yield("iden",tok)
    end
end

--- create a plain token iterator from a string or file-like object.
-- @string s the string
-- @tab matches an optional match table (set of pattern-action pairs)
-- @tab[opt] filter a table of token types to exclude, by default `{space=true}`
-- @tab[opt] options a table of options; by default, `{number=true,string=true}`,
-- which means convert numbers and strip string quotes.
function lexer.scan (s,matches,filter,options)
    --assert_arg(1,s,'string')
    local file = type(s) ~= 'string' and s
    filter = filter or {space=true}
    options = options or {number=true,string=true}
    if filter then
        if filter.space then filter[wsdump] = true end
        if filter.comments then
            filter[cdump] = true
        end
    end
    if not matches then
        if not plain_matches then
            plain_matches = {
                {WSPACE,wsdump},
                {NUMBER3,ndump},
                {IDEN,plain_vdump},
                {NUMBER1,ndump},
                {NUMBER2,ndump},
                {STRING3,sdump},
                {STRING0,sdump},
                {STRING1,sdump},
                {'^.',tdump}
            }
        end
        matches = plain_matches
    end
    local function lex ()
        if type(s)=='string' and s=='' then return end
        local findres,i1,i2,idx,res1,res2,tok,pat,fun,capt
        local line = 1
        if file then s = file:read()..'\n' end
        local sz = #s
        local idx = 1
        --print('sz',sz)
        while true do
            for _,m in ipairs(matches) do
                pat = m[1]
                fun = m[2]
                findres = { strfind(s,pat,idx) }
                i1 = findres[1]
                i2 = findres[2]
                if i1 then
                    tok = strsub(s,i1,i2)
                    idx = i2 + 1
                    if not (filter and filter[fun]) then
                        lexer.finished = idx > sz
                        res1,res2 = fun(tok,options,findres)
                    end
                    if res1 then
                        local tp = type(res1)
                        -- insert a token list
                        if tp=='table' then
                            yield('','')
                            for _,t in ipairs(res1) do
                                yield(t[1],t[2])
                            end
                        elseif tp == 'string' then -- or search up to some special pattern
                            i1,i2 = strfind(s,res1,idx)
                            if i1 then
                                tok = strsub(s,i1,i2)
                                idx = i2 + 1
                                yield('',tok)
                            else
                                yield('','')
                                idx = sz + 1
                            end
                            --if idx > sz then return end
                        else
                            yield(line,idx)
                        end
                    end
                    if idx > sz then
                        if file then
                            --repeat -- next non-empty line
                                line = line + 1
                                s = file:read()
                                if not s then return end
                            --until not s:match '^%s*$'
                            s = s .. '\n'
                            idx ,sz = 1,#s
                            break
                        else
                            return
                        end
                    else break end
                end
            end
        end
    end
    return wrap(lex)
end

local function isstring (s)
    return type(s) == 'string'
end

--- insert tokens into a stream.
-- @param tok a token stream
-- @param a1 a string is the type, a table is a token list and
-- a function is assumed to be a token-like iterator (returns type & value)
-- @string a2 a string is the value
function lexer.insert (tok,a1,a2)
    if not a1 then return end
    local ts
    if isstring(a1) and isstring(a2) then
        ts = {{a1,a2}}
    elseif type(a1) == 'function' then
        ts = {}
        for t,v in a1() do
            append(ts,{t,v})
        end
    else
        ts = a1
    end
    tok(ts)
end

--- get everything in a stream upto a newline.
-- @param tok a token stream
-- @return a string
function lexer.getline (tok)
    local t,v = tok('.-\n')
    return v
end

--- get current line number.
-- Only available if the input source is a file-like object.
-- @param tok a token stream
-- @return the line number and current column
function lexer.lineno (tok)
    return tok(0)
end

--- get the rest of the stream.
-- @param tok a token stream
-- @return a string
function lexer.getrest (tok)
    local t,v = tok('.+')
    return v
end

--- get the Lua keywords as a set-like table.
-- So `res["and"]` etc would be `true`.
-- @return a table
function lexer.get_keywords ()
    if not lua_keyword then
        lua_keyword = {
            ["and"] = true, ["break"] = true,  ["do"] = true,
            ["else"] = true, ["elseif"] = true, ["end"] = true,
            ["false"] = true, ["for"] = true, ["function"] = true,
            ["if"] = true, ["in"] = true,  ["local"] = true, ["nil"] = true,
            ["not"] = true, ["or"] = true, ["repeat"] = true,
            ["return"] = true, ["then"] = true, ["true"] = true,
            ["until"] = true,  ["while"] = true
        }
    end
    return lua_keyword
end

--- create a Lua token iterator from a string or file-like object.
-- Will return the token type and value.
-- @string s the string
-- @tab[opt] filter a table of token types to exclude, by default `{space=true,comments=true}`
-- @tab[opt] options a table of options; by default, `{number=true,string=true}`,
-- which means convert numbers and strip string quotes.
function lexer.lua(s,filter,options)
    filter = filter or {space=true,comments=true}
    lexer.get_keywords()
    if not lua_matches then
        lua_matches = {
            {WSPACE,wsdump},
            {NUMBER3,ndump},
            {IDEN,lua_vdump},
            {NUMBER4,ndump},
            {NUMBER5,ndump},
            {STRING3,sdump},
            {STRING0,sdump},
            {STRING1,sdump},
            {'^%-%-%[(=*)%[.-%]%1%]',cdump},
            {'^%-%-.-\n',cdump},
            {'^%[(=*)%[.-%]%1%]',sdump_l},
            {'^==',tdump},
            {'^~=',tdump},
            {'^<=',tdump},
            {'^>=',tdump},
            {'^%.%.%.',tdump},
            {'^%.%.',tdump},
            {'^//',tdump},
            {'^.',tdump}
        }
    end
    return lexer.scan(s,lua_matches,filter,options)
end

--- create a C/C++ token iterator from a string or file-like object.
-- Will return the token type type and value.
-- @string s the string
-- @tab[opt] filter a table of token types to exclude, by default `{space=true,comments=true}`
-- @tab[opt] options a table of options; by default, `{number=true,string=true}`,
-- which means convert numbers and strip string quotes.
function lexer.cpp(s,filter,options)
    filter = filter or {comments=true}
    if not cpp_keyword then
        cpp_keyword = {
            ["class"] = true, ["break"] = true,  ["do"] = true, ["sizeof"] = true,
            ["else"] = true, ["continue"] = true, ["struct"] = true,
            ["false"] = true, ["for"] = true, ["public"] = true, ["void"] = true,
            ["private"] = true, ["protected"] = true, ["goto"] = true,
            ["if"] = true, ["static"] = true,  ["const"] = true, ["typedef"] = true,
            ["enum"] = true, ["char"] = true, ["int"] = true, ["bool"] = true,
            ["long"] = true, ["float"] = true, ["true"] = true, ["delete"] = true,
            ["double"] = true,  ["while"] = true, ["new"] = true,
            ["namespace"] = true, ["try"] = true, ["catch"] = true,
            ["switch"] = true, ["case"] = true, ["extern"] = true,
            ["return"] = true,["default"] = true,['unsigned']  = true,['signed'] = true,
            ["union"] =  true, ["volatile"] = true, ["register"] = true,["short"] = true,
        }
    end
    if not cpp_matches then
        cpp_matches = {
            {WSPACE,wsdump},
            {PREPRO,pdump},
            {NUMBER3,ndump},
            {IDEN,cpp_vdump},
            {NUMBER4,ndump},
            {NUMBER5,ndump},
            {STRING3,sdump},
            {STRING1,chdump},
            {'^//.-\n',cdump},
            {'^/%*.-%*/',cdump},
            {'^==',tdump},
            {'^!=',tdump},
            {'^<=',tdump},
            {'^>=',tdump},
            {'^->',tdump},
            {'^&&',tdump},
            {'^||',tdump},
            {'^%+%+',tdump},
            {'^%-%-',tdump},
            {'^%+=',tdump},
            {'^%-=',tdump},
            {'^%*=',tdump},
            {'^/=',tdump},
            {'^|=',tdump},
            {'^%^=',tdump},
            {'^::',tdump},
            {'^.',tdump}
        }
    end
    return lexer.scan(s,cpp_matches,filter,options)
end

--- get a list of parameters separated by a delimiter from a stream.
-- @param tok the token stream
-- @string[opt=')'] endtoken end of list. Can be '\n'
-- @string[opt=','] delim separator
-- @return a list of token lists.
function lexer.get_separated_list(tok,endtoken,delim)
    endtoken = endtoken or ')'
    delim = delim or ','
    local parm_values = {}
    local level = 1 -- used to count ( and )
    local tl = {}
    local function tappend (tl,t,val)
        val = val or t
        append(tl,{t,val})
    end
    local is_end
    if endtoken == '\n' then
        is_end = function(t,val)
            return t == 'space' and val:find '\n'
        end
    else
        is_end = function (t)
            return t == endtoken
        end
    end
    local token,value
    while true do
        token,value=tok()
        if not token then return nil,'EOS' end -- end of stream is an error!
        if is_end(token,value) and level == 1 then
            append(parm_values,tl)
            break
        elseif token == '(' then
            level = level + 1
            tappend(tl,'(')
        elseif token == ')' then
            level = level - 1
            if level == 0 then -- finished with parm list
                append(parm_values,tl)
                break
            else
                tappend(tl,')')
            end
        elseif token == delim and level == 1 then
            append(parm_values,tl) -- a new parm
            tl = {}
        else
            tappend(tl,token,value)
        end
    end
    return parm_values,{token,value}
end

--- get the next non-space token from the stream.
-- @param tok the token stream.
function lexer.skipws (tok)
    local t,v = tok()
    while t == 'space' do
        t,v = tok()
    end
    return t,v
end

local skipws = lexer.skipws

--- get the next token, which must be of the expected type.
-- Throws an error if this type does not match!
-- @param tok the token stream
-- @string expected_type the token type
-- @bool no_skip_ws whether we should skip whitespace
function lexer.expecting (tok,expected_type,no_skip_ws)
    assert_arg(1,tok,'function')
    assert_arg(2,expected_type,'string')
    local t,v
    if no_skip_ws then
        t,v = tok()
    else
        t,v = skipws(tok)
    end
    if t ~= expected_type then error ("expecting "..expected_type,2) end
    return v
end

return lexer
]===]

local lexer = load( pl_lexer, 'lexer.lua' )()

local enctab = {
  [ " " ] = '0', -- 221208
  [ "." ] = '100', -- 62755
  [ "=" ] = '1010', -- 29673
  [ "end" ] = '1011000', -- 3844
  [ "if" ] = '10110010', -- 1967
  [ "==" ] = '10110011', -- 2003
  [ "function" ] = '101101000', -- 1006
  [ "local" ] = '101101001', -- 1096
  [ "<=" ] = '1011010100', -- 541
  [ "while" ] = '1011010101', -- 557
  [ ":" ] = '10110101100', -- 289
  [ "<" ] = '10110101101', -- 290
  [ "-" ] = '10110101110', -- 304
  [ "for" ] = '10110101111', -- 314
  [ "then" ] = '10110110', -- 2637
  [ "and" ] = '1011011100', -- 644
  [ ">=" ] = '10110111010', -- 319
  [ ">" ] = '10110111011', -- 326
  [ "elseif" ] = '1011011110', -- 670
  [ "else" ] = '10110111110', -- 381
  [ "%" ] = '101101111110', -- 198
  [ "nil" ] = '101101111111', -- 201
  [ "self" ] = '10111', -- 24196
  [ "\
" ] = '1100', -- 45578
  [ "true" ] = '11010000', -- 2799
  [ "," ] = '11010001', -- 2815
  [ "[" ] = '1101001', -- 6744
  [ ")" ] = '110101', -- 12925
  [ "(" ] = '110110', -- 12925
  [ "]" ] = '1101110', -- 6744
  [ "false" ] = '11011110', -- 3512
  [ "{" ] = '11011111000', -- 429
  [ "}" ] = '11011111001', -- 429
  [ "do" ] = '1101111101', -- 871
  [ "+" ] = '1101111110', -- 900
  [ "~=" ] = '1101111111000', -- 99
  [ "return" ] = '11011111110010', -- 55
  [ "/" ] = '1101111111001100', -- 13
  [ "^" ] = '1101111111001101', -- 14
  [ "#" ] = '110111111100111', -- 28
  [ "or" ] = '110111111101', -- 220
  [ "not" ] = '110111111110', -- 244
  [ "//" ] = '1101111111110', -- 121
  [ "in" ] = '110111111111100', -- 34
  [ "*" ] = '110111111111101', -- 35
  [ "until" ] = '11011111111111000', -- 7
  [ "|" ] = '1101111111111100100000', -- 0
  [ "goto" ] = '1101111111111100100001', -- 0
  [ "break" ] = '1101111111111100100010', -- 0
  [ ">>" ] = '11011111111111001000110', -- 0
  [ "\9" ] = '11011111111111001000111', -- 0
  [ "~" ] = '11011111111111001001000', -- 0
  [ "::" ] = '11011111111111001001001', -- 0
  [ ";" ] = '11011111111111001001010', -- 0
  [ "eof" ] = '11011111111111001001011', -- 0
  [ "<<" ] = '11011111111111001001100', -- 0
  [ "\11" ] = '11011111111111001001101', -- 0
  [ "\13" ] = '11011111111111001001110', -- 0
  [ "\12" ] = '11011111111111001001111', -- 0
  [ "..." ] = '1101111111111100101', -- 4
  [ "repeat" ] = '110111111111110011', -- 7
  [ "&" ] = '1101111111111101', -- 20
  [ ".." ] = '110111111111111', -- 39
  [ "literal" ] = '111', -- 116390
}

local function errorout( ... )
  local args = { ... }
  local format = args[ 1 ]
  table.remove( args, 1 )
  io.stderr:write( string.format( format, args ), '\n' )
  os.exit( 1 )
end

local function addbits( encoded, bits )
  for i = 1, #bits do
    encoded[ #encoded + 1 ] = bits:sub( i, i ) + 0
  end
end

local function addliteral( encoded, str )
  if #encoded % 8 == 0 then
    addbits( encoded, '1' )
  end
  
  for i = 1, #str do
    local k = str:byte( i, i )
    
    for j = 7, 0, -1 do
      if bit32.band( k, bit32.lshift( 1, j ) ) ~= 0 then
        addbits( encoded, '1' )
      else
        addbits( encoded, '0' )
      end
    end
  end
  
  addbits( encoded, '00000000' )
end

local function bitstobyte( encoded, i )
  local bit = 128
  local byte = 0
  
  for j = 0, 7 do
    byte = byte + encoded[ i + j ] * bit
    bit = bit / 2
  end
  
  return string.char( byte )
end

local function encode( source )
  local encoded = {}
  
  for token, lexeme in lexer.lua( source, {} ) do
    local bits
    
    if token == 'space' then
      for i = 1, #lexeme do
        addbits( encoded, enctab[ lexeme:sub( i, i ) ] )
      end
    elseif token == 'keyword' then
      addbits( encoded, enctab[ lexeme ] )
    elseif token == 'iden' then
      addbits( encoded, enctab.literal )
      addliteral( encoded, lexeme )
    elseif token == 'number' then
      addbits( encoded, enctab.literal )
      addliteral( encoded, tostring( lexeme ) )
    elseif token == 'string' then
      addbits( encoded, enctab.literal )
      addliteral( encoded, string.format( '%q', lexeme ) )
    elseif token == 'comment' then
      addbits( encoded, enctab.literal )
      addliteral( encoded, lexeme )
    else
      addbits( encoded, enctab[ lexeme ] )
    end
  end
  
  addbits( encoded, enctab.eof )
  
  while #encoded %8 ~= 0 do
    addbits( encoded, '0' )
  end
  
  local s = {}
  
  for i = 1, #encoded, 8 do
    s[ #s + 1 ] = bitstobyte( encoded, i )
  end
  
  return table.concat( s )
end

local function main( args )
  if #args ~= 2 then
    io.write( 'Usage: lua bsenc.lua <input.lua> <output.bs>\n' )
    return 0
  end
  
  local file, err = io.open( args[ 1 ] )
  
  if not file then
    errorout( 'Error opening %s', args[ 1 ] )
  end
  
  local source = file:read( '*a' )
  file:close()
  
  if not source then
    errorout( 'Could not read from %s', args[ 1 ] )
  end
  
  local encoded = encode( source )
  
  file, err = io.open( args[ 2 ], 'wb' )
  
  if not file then
    errorout( 'Error opening %s', args[ 2 ] )
  end
  
  file:write( encoded )
  file:close()
end

return main( arg )
  